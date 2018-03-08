/*
==============================================================================

FileSelectWindow.h
Created: 20 Feb 2018 10:41:09pm
Author:  User

==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class FileSelectWindow : public Component, public Button::Listener,
	public ChangeBroadcaster
{
public:
	FileSelectWindow()
	{
		//Allocate the memory for the open buttons...
		for (int i = 0; i <= 3; i++) {
			openButtons[i] = new TextButton;
		}
		//...and the close buttons
		for (int i = 0; i <= 3; i++) {
			closeButtons[i] = new TextButton;
		}
		//Set the parameters and make visible the open buttons...
		for (int i = 0; i <= 3; i++) {
			if (openButtons[i] != nullptr) {
				addAndMakeVisible(openButtons[i]);
				openButtons[i]->setButtonText("+");
				openButtons[i]->addListener(this);
				openButtons[i]->setColour(TextButton::buttonColourId, Colours::green);
				openButtons[i]->setEnabled(true);
			}
			else {
				break;
			}
		}
		//...and the cloose buttons
		for (int i = 0; i <= 3; i++) {
			if (closeButtons[i] != nullptr) {
				addAndMakeVisible(closeButtons[i]);
				closeButtons[i]->setButtonText("-");
				closeButtons[i]->addListener(this);
				closeButtons[i]->setColour(TextButton::buttonColourId, Colours::red);
				closeButtons[i]->setEnabled(true);
			}
			else {
				break;
			}
		}

	}

	~FileSelectWindow() {
		//Delete the buttons
		for (int i = 0; i <= 3; i++) {
			delete openButtons[i];
			delete closeButtons[i];
		}

	}

	//Return a pointer for the open buttons for listeners etc...
	Button *getOpenButton(int channel) {
		return openButtons[channel];
	}
	//...and for the close buttons
	Button *getCloseButton(int channel) {
		return closeButtons[channel];
	}

	void buttonClicked(Button *button) override {
		//Not used currently....may be needed later tho
	}

	void paint(Graphics &g) override {
		//Fill with colour and set the buttonss sizes based on the local bounds
		g.fillAll(Colours::grey);
		for (int i = 0; i <= 3; i++) {
			openButtons[i]->setBounds(10, ((i + 1) * 125) - 85, 25, 25);
		}
		for (int i = 0; i <= 3; i++) {
			closeButtons[i]->setBounds(10, ((i + 1) * 125) - 35, 25, 25);
		}
	}

private:
	//An array of button pointers to allocate the memory for open and close buttons
	TextButton *openButtons[4];
	TextButton *closeButtons[4];
};
