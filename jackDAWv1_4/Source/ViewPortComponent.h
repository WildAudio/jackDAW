/*
==============================================================================

ViewportComponent.h
Created: 13 Feb 2018 10:04:23pm
Author:  User

==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
//#include "EditWindow.h"
#include "TrackContainer.h"

class ViewportComponent : public Viewport
{
public:
	ViewportComponent(AudioTransportSource *transportSourcesToUse[4], AudioTransportSource &timerToUse, File *filesToUse[4])
		: //transportSource(transportSourceToUse),
		//transportSource2(transportSourceToUse2),
		//transportSource3(transportSourceToUse3),
		//transportSource4(transportSourceToUse4),
		timer(timerToUse)
	{
		//Set the file pointers created to the file array passed
		for (int i = 0; i <= 3; i++) {
			files[i] = filesToUse[i];
		}

		for (int i = 0; i <= 3; i++) {
			transportSources[i] = transportSourcesToUse[i];
		}

		/*Allocate the memory for the Track Container, which will house the Edit Window and
		the File Select Window*/
		trackContainer = new TrackContainer(transportSources, timer, files);

		//Set the size and the viewed component of the Port to be the Track Container
		setSize(700, 400);
		setViewedComponent(trackContainer, true);
		addMouseListener(this, true);
	}

	~ViewportComponent()
	{

	}

	//Function to return a pointer to the TrackContainer to set listeners etc.
	TrackContainer *getTrackContainer() {
		return trackContainer;
	}

	void paint(Graphics &g) {
		//Doesn't need filling as the view component has already bee set
	}

private:
	//Create a Scoped Pointer to allocate the memory for a Track Container
	ScopedPointer<TrackContainer> trackContainer;

	/*AudioTransportSource &transportSource;
	AudioTransportSource &transportSource2;
	AudioTransportSource &transportSource3;
	AudioTransportSource &transportSource4;*/
	AudioTransportSource &timer;

	//An array of file pointers to set as the files passed in the constructor
	AudioTransportSource *transportSources[4];
	File *files[4];

};