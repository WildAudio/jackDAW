/*
==============================================================================

MixWindow.h
Created: 12 Feb 2018 10:21:25pm
Author:  User

==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include <sstream>

using namespace std;



class MixWindow : public Component, public Slider::Listener, public Button::Listener, public ChangeBroadcaster

{
public:
	MixWindow()
	{
		//Allocate memory for the volume sliders...
		for (int i = 0; i <= 3; i++) {
			sliders[i] = new Slider;
		}
		//...and the channels select buttons
		for (int i = 0; i <= 3; i++) {
			buttons[i] = new TextButton;
		}
		//...and the panner rotaries (again, could probably do this is one for loop now?)
		for (int i = 0; i <= 3; i++) {
			pans[i] = new Slider;
		}

		//Debug stuff
		addAndMakeVisible(displayLabel);
		displayLabel.setColour(Label::backgroundColourId, Colours::black);
		displayLabel.setColour(Label::textColourId, Colours::white);

		addAndMakeVisible(displayLabel2);
		displayLabel2.setColour(Label::backgroundColourId, Colours::black);
		displayLabel2.setColour(Label::textColourId, Colours::white);

		//Make visible and set the values for the volume sliders...
		for (int i = 0; i <= 3; i++) {
			if (sliders[i] != nullptr) {
				addAndMakeVisible(sliders[i]);
				sliders[i]->setSliderStyle(Slider::LinearVertical);
				sliders[i]->setRange(0.05, 1.0);
				sliders[i]->setValue(1.0);
				sliders[i]->setTextBoxStyle(Slider::TextBoxBelow, false, 100, 20);
				sliders[i]->addListener(this);
			}
			else {
				break;
			}
		}
		//...and panner rotaries...
		for (int i = 0; i <= 3; i++) {
			if (pans[i] != nullptr) {
				addAndMakeVisible(pans[i]);
				pans[i]->setSliderStyle(Slider::RotaryVerticalDrag);
				pans[i]->setRange(-0.9, 1.0);
				pans[i]->setValue(0.0);
				pans[i]->setTextBoxStyle(Slider::TextBoxAbove, false, 75, 15);
				pans[i]->addListener(this);
			}
			else {
				break;
			}
		}
		//...and channel select buttons
		for (int i = 0; i <= 3; i++) {
			if (buttons[i] != nullptr) {
				addAndMakeVisible(buttons[i]);
				buttons[i]->setButtonText("-");
				buttons[i]->addListener(this);
				buttons[i]->setColour(TextButton::buttonColourId, Colours::yellow);
				buttons[i]->setEnabled(true);
			}
			else {
				break;
			}
		}

	}
	//Call the release resources function in the destructor
	~MixWindow()
	{
		releaseResources();
	}

	//==============================================================================

	
	void releaseResources() {
		/*Manage all the allocated memory for the volume sliders, panner rotaries and channel select buttons.
		TODO: Again, more elegant to do in one for loop at some point*/
		for (int i = 0; i <= 3; i++) {
			if (sliders[i] != nullptr) {
				delete sliders[i];
			}
		}
		for (int i = 0; i <= 3; i++) {
			if (buttons[i] != nullptr) {
				delete buttons[i];
			}
		}
		for (int i = 0; i <= 3; i++) {
			if (pans[i] != nullptr) {
				delete pans[i];
			}
		}
	}

	void paint(Graphics& g) override
	{
		g.fillAll(Colours::grey);
		//Draw all of the components based on the local bounds of the Mix Window
		for (int i = 0; i <= 3; i++) {
			if (sliders[i] != nullptr) {
				sliders[i]->setBounds(((i + 1) * (getWidth()/10)) - 45, 80, 50, getHeight()/1.50);
			}
			else {
				break;
			}
		}
		for (int i = 0; i <= 3; i++) {
			if (buttons[i] != nullptr) {
				buttons[i]->setBounds(((i + 1) * (getWidth()/10)) -5, 100, 10, 10);
			}
			else {
				break;
			}
		}
		for (int i = 0; i <= 3; i++) {
			if (pans[i] != nullptr) {
				pans[i]->setBounds(((i + 1) * (getWidth()/10)) - 55, 10, 75, 75);
			}
		}

		//displayLabel.setBounds(300, 50, 100, 50);
	}

	void resized() override
	{
		//Update the bounds when resized and redraw
		repaint();
	}

	void sliderValueChanged(Slider* slider) override
	{
		//Debug output
		/*for (int i = 0; i <= 3; i++) {
			if (slider == sliders[i]) {
				ss.str(" ");
				ss << sliders[i]->getValue();
				displayLabel.setText(ss.str(), sendNotification);
			}
		}*/
	}

	//Get functions for all sliders and buttons so that listeners can be added in the main component window
	Slider *getSlider(int sliderToUse) {
		return sliders[sliderToUse];
	}

	Slider *getPanner(int sliderToUse) {
		return pans[sliderToUse];
	}

	Button *getButton(int buttonToUse) {
		return buttons[buttonToUse];
	}


	void buttonClicked(Button* button) override
	{
		//Old debug output. May well still be useful at some point, so keep for the moment
		/*for (int i = 0; i <= 3; i++) {
			if (button == buttons[i]) {

			}
		}*/
	}

private:

	stringstream ss, ss2;

	Label displayLabel, displayLabel2;

	//Arrays of pointers to create all of the volume sliders...
	Slider *sliders[4];
	//...and panner rotaries...
	Slider *pans[4];
	//...and channel select buttons
	TextButton *buttons[4];

	//JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
};



//#endif  // MIXWINDOW_H_INCLUDED
