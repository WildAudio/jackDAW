/*
==============================================================================

AudioTrack.h
Created: 5 Feb 2018 6:41:20pm
Author:  User

==============================================================================
*/

#pragma once

//#ifndef AUDIOTRACK_H_INCLUDED
//#define AUDIOTRACK_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformRegion.h"

class AudioTrack : public  Component,
	public ChangeBroadcaster,
	public ChangeListener
{
public:
	
	AudioTrack(double pixelsToUse, File &fileToUse, AudioTransportSource &transportSourceToUse)
		:activeRegion(0),              //Initialise both the active region and number of regions to 0
		numberOfRegions(0),
		pixelsPerSecond(pixelsToUse),  //Initialise the pixels per second based on the dimensions of the Edit Window
		activeRegionsFound(false),     //Set active regions to non found initially
		thumbnailCache(5),             //Initialise the thumbnail cache
		file(fileToUse),               //The file passed is set as the file reference created...
		transportSource(transportSourceToUse)     //...and the transport source reference as the transport source passed
	{
		//Register the formats for the Regions's thumbnail to use
		formatManager.registerBasicFormats();

		//Create the initial Region, based off the whole file attached to the audio transport source. Pass the length to use, X start position and end position based on the transport source multiplied by the pixels to use per second and whether it should be seen as active
		regions[0] = new WaveformRegion(512, formatManager, thumbnailCache, transportSource, 0.0, (transportSource.getLengthInSeconds() * pixelsPerSecond), 0.0, transportSource.getLengthInSeconds(), true, transportSource.getLengthInSeconds());
		regions[0]->setFile(file);
		regions[0]->setLengthToUse(transportSource.getLengthInSeconds());
		addAndMakeVisible(regions[0]);
		regions[0]->addChangeListener(this);         //Add a listener so that the region can inform the track of when it has been moved
	}

	~AudioTrack() {
		for (int i = 0; i <= numberOfRegions; i++) {
			delete regions[i];             //Delete the regions in the destructor
		}
	}


	//Returns a refernce to the transport source attached to the track
	AudioTransportSource& getAudioTransportSource() {
		return transportSource;
	}

	//If a region is moved on the track, get the number of the region moved and send a change message to the Edit Window
	void changeListenerCallback(ChangeBroadcaster* source) override {
		for (int i = 0; i <= numberOfRegions; i++) {
			if (regions[i] != nullptr) {
				if (source == regions[i]) {
					sendChangeMessage();
					regionMoved = i;
				}
			}
		}
	}

	/*Set the pixels per second externally, for example if the window containing the Track is 
	resized*/
	void setPixelsPerSecond(double setPixels) {
		pixelsPerSecond = setPixels;
	}

	//Return a reference of the File attached to the Track
	File &getFile() {
		return file;
	}

	/*WaveformRegion *getWaveform(int waveformToGet) {
		return regions[waveformToGet];
	}*/

	//Return the pixels per second currently being used - useful for debug
	double getPixelsPerSecond() {
		return pixelsPerSecond;
	}

	//Returns whether there are acive regions being found - again useful for debug
	bool getActiveRegionsFound() {
		return activeRegionsFound;
	}

	/*Returns a pointer to any of the regions required, if not available a null pointer is 
	returned*/
	WaveformRegion *getRegions(int i) {
		if (regions[i] != nullptr) {
			return regions[i];
		}
		else
			return nullptr;
	}

	void paint(Graphics &g) {
		/*Draw all of the regions that have been created and are flagged as on-screen, based on
		the X start and end attributes that it has and the Edit Window containing it*/
		for (int i = 0; i <= numberOfRegions; i++) {
			if (regions[i] != nullptr && regions[i]->getOnScreen() == true) {
			regions[i]->setBounds(regions[i]->getOffset(), 0, (regions[i]->getXValueEnd() - regions[i]->getOffset()), 120);
		}
		}
	}

	void resize(Graphics &g) {
		//Re-set the local bounds and re-draw
		repaint();
	}

	void setActiveRegions(int playHeadX) {
		activeRegionsFound = false; //Assume that no active regions are to be found

		/*Iterate over all of the created regions, check whether they are flagged as on-screen
		and whether the passed playhead position is in-between the beginning and end of their X
		co-ordinates. If so, set the region as active, the activeRegionsFound bool to true and
		the active region to pass integer as the currently active region*/
		for (int i = 0; i <= numberOfRegions; i++) {
			if (regions[i] != nullptr) {
				if (regions[i]->getOnScreen() == true && playHeadX >= regions[i]->getOffset() && playHeadX < (regions[i]->getOffset() + regions[i]->getWidth())) {
					regions[i]->setActive(true, true);
					activeRegionsFound = true;
					activeRegion = i;
				}
				//If none of these criteria are met, set the region's active status to false
				else {
					regions[i]->setActive(false, true);
				}
			}
		}
	}

	//Return the active region to be used externally...
	int getActiveRegion() {
		return activeRegion;
	}
	//...and the total number of regions that have been created...
	int getNumberOfRegions() {
		return numberOfRegions;
	}
	//...and the region that was just moved
	int getRegionMoved() {
		return regionMoved;
	}

	void reposition(int playHeadX, AudioTransportSource &timerToUse) {
		//Set the active regions
		setActiveRegions(playHeadX);

		//If active regions are found, reposition the transport source attached to the Track based on the X position of the playhead co-ordinates passed relative to the region's dimensions 
		if (activeRegionsFound == true) {
			regions[activeRegion]->getAudioTransportSource().setPosition(regions[activeRegion]->getStartTime() + ((playHeadX - (regions[activeRegion]->getOffset())) / pixelsPerSecond));
			//If the timer passed is playing, then begin the track playing from this position, otherwise keep the transport stopped
			if (timerToUse.isPlaying()) {
				regions[activeRegion]->getAudioTransportSource().start();
			}
		}
		//If there are no active regions on this track at the current playhead position passed, then stop the transport
		else {
			regions[0]->getAudioTransportSource().stop();
		}

	}

	//Re-sets the file attached to the track when it is updated
	void setFile(String fileToPass) {
		file = fileToPass;
	}

	/*Updates the start and end times, as well as the start and end X co-ordinates of the initial region
	when the transport source/file are changed*/
	void updateStartAndEndTimes() {
		regions[0]->setStartTime(0.0);
		//regions[0]->setEndTime(transportSource.getLengthInSeconds());
		regions[0]->setOffset(0);
		regions[0]->setXValueEnd(regions[0]->getEndTime() * pixelsPerSecond);
	}

	/*Creates two new regions based on the position of the playhead passed at that point over the
	region to be broken*/
	void breakRegion(int playHeadX, AudioTransportSource &timerToUse) {

		setActiveRegions(playHeadX);
		if (activeRegionsFound == true) { //Set active regions, and if active regions are found...
			regions[activeRegion]->setActive(false, false);
			regions[activeRegion]->setOnScreen(false);
			removeChildComponent(regions[activeRegion]);//De-activate, set off-screen and remove the parent region from the Track
			//Create two new regions with parameters based on the playhead's position over the parent region
			regions[numberOfRegions + 1] = new WaveformRegion(512, formatManager, thumbnailCache, transportSource, regions[activeRegion]->getOffset(), playHeadX, regions[activeRegion]->getStartTime(), transportSource.getCurrentPosition(), true, transportSource.getLengthInSeconds() - (transportSource.getLengthInSeconds() - timerToUse.getCurrentPosition()));
			regions[numberOfRegions + 2] = new WaveformRegion(512, formatManager, thumbnailCache, transportSource, playHeadX + 1, regions[activeRegion]->getXValueEnd(), transportSource.getCurrentPosition(), transportSource.getLengthInSeconds(), false, transportSource.getLengthInSeconds() - timerToUse.getCurrentPosition());
			//Set the file based on the parent, add, make visible, add listener and push to the back in order to be behind the playhead for the first region...
			regions[numberOfRegions + 1]->setFile(file);
			addAndMakeVisible(regions[numberOfRegions + 1]);
			regions[numberOfRegions + 1]->toBack();
			regions[numberOfRegions + 1]->addChangeListener(this);
			//...and for the second
			regions[numberOfRegions + 2]->setFile(file);
			addAndMakeVisible(regions[numberOfRegions + 2]);
			regions[numberOfRegions + 2]->toBack();
			regions[numberOfRegions + 2]->addChangeListener(this);
			//Update the number of regions currently on the Track
			numberOfRegions += 2;
		}
	}

	void deleteRegions() {
		for (int i = 0; i <= numberOfRegions; i++) {
			regions[i]->getThumbnail()->clear();	
		}
		//regions[0]->getThumbnail()->clear();
		removeAllChildren();
		addAndMakeVisible(regions[0]);
		regions[0]->addChangeListener(this);
		regions[0]->setOnScreen(true);
		regions[0]->setActive(true, false);
		numberOfRegions = 0;
		activeRegion = 0;
		repaint();
	}

	void openButtonClicked()
	{
		
	}

private:
	int activeRegion, numberOfRegions, regionMoved;
	double pixelsPerSecond;
	bool activeRegionsFound;

	AudioFormatManager formatManager;
	ScopedPointer<AudioFormatReaderSource> readerSource;
	AudioTransportSource &transportSource;
	AudioThumbnailCache thumbnailCache;

	//A reference for the File that is to be attached to the track
	File &file;

	//An array of pointers to regions, just waiting to be filled with memory!
	WaveformRegion *regions[10];

	/*void setRegionSources(AudioFormatManager &formatManagerToUse, ScopedPointer<AudioFormatReaderSource> &readerSourceToUse, AudioTransportSource &transportSourceToUse, File &fileToUse) {
		AudioFormatReader* readerToUse = formatManagerToUse.createReaderFor(fileToUse);
		ScopedPointer<AudioFormatReaderSource> newSource = new AudioFormatReaderSource(readerToUse, true);
		transportSourceToUse.setSource(newSource, 0, nullptr, readerToUse->sampleRate);
		readerSourceToUse = newSource.release();
	}*/

	//JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};