/*
==============================================================================

TrackContainer.h
Created: 19 Feb 2018 2:28:27pm
Author:  User

==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
//#include "ViewportComponent.h"
#include "EditWindow.h"
#include "FileSelectWindow.h"

//static ScopedPointer<ApplicationCommandManager> applicationCommandManager;

class TrackContainer : public Component, public ChangeListener
{
public:
	TrackContainer(AudioTransportSource *transportSourcesToUse[4], AudioTransportSource &timerToUse, File *filesToUse[4])
		://transportSource(transportSourceToUse),
		//transportSource2(transportSourceToUse2),
		//transportSource3(transportSourceToUse3),
		//transportSource4(transportSourceToUse4),
		timer(timerToUse)
	{
		setSize(1054, 1000);

		addMouseListener(this, true);

		//Set the file pointers to the files passed
		for (int i = 0; i <= 3; i++) {
			files[i] = filesToUse[i];
		}

		for (int i = 0; i <= 3; i++) {
			transportSources[i] = transportSourcesToUse[i];
		}

		//Allocate the memory for the File Select Window and add a listener
		fileSelectWindow = new FileSelectWindow;
		addAndMakeVisible(fileSelectWindow);
		fileSelectWindow->addChangeListener(this);

		//Allocate the memory for the Edit Window and make it visible
		editWindow = new EditWindow(transportSources, timer, files);
		addAndMakeVisible(editWindow);

	}

	~TrackContainer()
	{
		//Nothing to delete as all pointers are Scoped
	}

	//Return a pointer to the Edit Window to add listeners etc.... 
	EditWindow *getEditWindow() {
		return editWindow;
	}

	//...and to the File Select Window
	FileSelectWindow *getFileSelectWindow() {
		return fileSelectWindow;
	}


	void paint(Graphics &g) override
	{
		/*Fill with colour and set the size of the components' size (won't be based on local
		bounds currently*/
		g.fillAll(Colours::grey);
		fileSelectWindow->setBounds(0, 0, 50, 1000);
		editWindow->setBounds(50, 0, 1004, 1000);
	}

	void resized() override
	{
		//No need to re-size as this will be bigger than the ViewPort containing it
	}

	void changeListenerCallback(ChangeBroadcaster *source) override {
		//Not currently needed
	}


private:
	//Scoped pointers to allocate memory for the File Select Window and the Edit Window
	ScopedPointer<FileSelectWindow> fileSelectWindow;
	ScopedPointer<EditWindow> editWindow;

	/*AudioTransportSource &transportSource;
	AudioTransportSource &transportSource2;
	AudioTransportSource &transportSource3;
	AudioTransportSource &transportSource4;*/
	AudioTransportSource &timer;

	//Array of file pointers to set as the passed files
	File *files[4];

	AudioTransportSource *transportSources[4];
};
