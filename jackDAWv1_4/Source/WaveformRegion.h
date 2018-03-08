

#pragma once

//#ifndef WAVEFORMREGION_H_INCLUDED
//#define WAVEFORMREGION_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class WaveformRegion : public Component,
	public ChangeBroadcaster,
	private ChangeListener
{
public:

	WaveformRegion(int sourceSamplesPerThumbnailSample,
		AudioFormatManager& formatManager,
		AudioThumbnailCache& cache, AudioTransportSource& transportSourceToUse,
		int xValueToUse, int xValueEndToUse, float startTimeToUse, float endTimeToUse,
		bool activeToUse, float lengthToUse)
		: xValue(xValueToUse),
		xValueEnd(xValueEndToUse),
		startTime(startTimeToUse),
		endTime(endTimeToUse),
		active(activeToUse),            //Initialise the region with values passed based on the parent relative to the playhead position, or the entire transport source if it is the initial parent region
		currentPosition(0.0),
		initialPosition(0.0),
		lengthToDisplay(0.0),           //Initialise positions and lengths to 0
		lengthPassedToUse(lengthToUse), //The amount of the transport source to be rendered
		thumbnail(sourceSamplesPerThumbnailSample, formatManager, cache), //Initialise the thumbnail compoinent created in the class
		transportMoved(0.0),
		mouseReleaseValue(0.0),
		offset(xValueToUse),            //The offset of the region on the Edit Window (basically the same as the X start value
		selected(false),                //Set the region to initially not be selected
		transportSource(transportSourceToUse),  //Set the transport source reference based on the transport source passed from the audio track
		onScreen(true)                  //Set the region to initially be on screen
	{
		thumbnail.addChangeListener(this);  //Add a change listener for when the thumbnail is updated or changed
	}

	~WaveformRegion()
	{

	}

	//==============================================================================

	void setFile(const File& file)
	{
		thumbnail.setSource(new FileInputSource(file)); //Set the thumbnail source as the file passed from the Audio Track
	}

	//Return the transport source attached to the Region
	AudioTransportSource& getAudioTransportSource() {
		return transportSource;
	}

	//Return whether the region is currently active
	bool getActive() {
		return active;
	}

	//Allows the Edit Window or Audio Track to set whether the region is currently active
	void setActive(bool isActive, bool wasChanged) {
		active = isActive;
		
		//sendChangeMessage();
	}

	

	//Returns whether the Region is currently on-screen
	bool getOnScreen() {
		return onScreen;
	}

	//Allows the Region's on-screen status to be set externally
	void setOnScreen(bool isVisible) {
		onScreen = isVisible;
	}

	//Return's the Region's start time
	float getStartTime() {
		return startTime;
	}

	//Allows the Region's start time to be set externally
	void setStartTime(float timeToUse) {
		startTime = timeToUse;
	}

	//Returns the Region's end time
	float getEndTime() {
		return endTime;
	}

	//Allows the Region's end time to be set externally
	void setEndTime(float timeToUse) {
		endTime = timeToUse;
	}

	//Returns the Region's end X value
	int getXValueEnd() {
		return xValueEnd;
	}

	//Allows the Region's end X value to be set externally
	void setXValueEnd(int valueToUse) {
		xValueEnd = valueToUse;
	}

	//Draws the Region based on whether or not a file is loaded
	void paint(Graphics& g) override
	{
		if (thumbnail.getNumChannels() == 0) {
			paintIfNoFileLoaded(g);
		}
		else {
			paintIfFileLoaded(g);
		}
	}

	void fillBlack(Graphics& g) {
		g.fillAll(Colours::black);
	}

	//If no file is loaded, this function runs indicating the state
	void paintIfNoFileLoaded(Graphics& g)
	{
		g.fillAll(Colours::white);
		g.setColour(Colours::darkgrey);
		g.drawFittedText("No File Loaded", getLocalBounds(), Justification::centred, 1.0f);
	}

	//Allows the Region's length to display to be set externally
	void setLengthToUse(double length) {
		lengthPassedToUse = length;
	}

	//Returns the length to be displayed in the region in seconds
	double getLengthtoUse() {
		return lengthPassedToUse;
	}

	//Draws the audio thumbnail with a black background if selected, white if not
	void paintIfFileLoaded(Graphics& g)
	{
		if (!selected) {
			g.fillAll(Colours::white);

			g.setColour(Colours::red);
			thumbnail.drawChannels(g, getLocalBounds(), startTime, endTime, 1.0f);
		}
		else {
			g.fillAll(Colours::black);

			g.setColour(Colours::red);
			thumbnail.drawChannels(g, getLocalBounds(), startTime, endTime, 1.0f);
		}
	}

	//Returns the X offset of the Region on the Edit Window
	int getOffset() {
		return offset;
	}

	//Allows the Region's X offset to be set extrenally
	void setOffset(float offsetToUse) {
		offset = offsetToUse;
	}


	int getMouseReleaseValue() {
		return initialPosition;
	}

	//Allows the Region's X value to be set externally
	void setxValue(int value) {
		xValue = value;
	}

	void setTransportMoved(int value) {
		transportMoved = value;
	}

	int getTransportMoved() {
		return transportMoved;
	}

	/*When the mouse is clicked, set the region as selected, and set the initial position to 
	be the X position of the mouse event*/
	void mouseDown(const MouseEvent &event) override
	{
		initialPosition = event.position.x;
		selected = true;
		repaint();
	}

	/*Update the region's X offset on the Edit Window and X end value in real-time when the
	mouse is dragged, sending a change message upwards to the Edit Window to update the 
	display*/
	void mouseDrag(const MouseEvent &event) override
	{
		currentPosition = event.position.x - initialPosition;
		offset += (currentPosition * 0.5);
		xValueEnd += (currentPosition * 0.5);
		sendChangeMessage();
	}

	void mouseUp(const MouseEvent& event) override
	{
		//currentPosition = event.position.x;

		//mouseReleaseValue = event.position.x;

		//transportSource.setPosition(0 - transportMoved);

		//offset += event.getDistanceFromDragStartX();

		//xValueEnd += event.getDistanceFromDragStartX();
		//if (initialPosition != currentPosition) {
		//sendChangeMessage();
		//}

	}

	//When the thumbail is updated or cahnged, call the thumbnail changed function
	void changeListenerCallback(ChangeBroadcaster* source) override
	{
		if (source == &thumbnail)
			thumbnailChanged();
	}

	//Returns a pointer to the thumbnail attached to the Region
	AudioThumbnail *getThumbnail() {
		return &thumbnail;
	}

private:
	//Repaints the thumbnail when a change message is received
	void thumbnailChanged() {
		repaint();
	}
	//Ints to be used in drawing and positioning the Region
	int xValue, xValueEnd, currentPosition, initialPosition, offset;
	//The thumbnail to be drawn in the Region
	AudioThumbnail thumbnail;
	//Floats to be used to return positions on the transport source and the length to display
	float lengthToDisplay, lengthPassedToUse, transportMoved, mouseReleaseValue, startTime, endTime;
	//Bools to set whether the region is selected, active and currently on-screen
	bool selected, active, onScreen;
	//A reference to the audio transport source passed from the Audio track
	AudioTransportSource &transportSource;


	//JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};

//#endif  // WAVEFORMREGION_H_INCLUDED
