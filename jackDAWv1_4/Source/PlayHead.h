#include "../JuceLibraryCode/JuceHeader.h"

class PlayHead : public Component

{
public:
	PlayHead()

	{

	}

	void paint(Graphics& g) override
	{

		g.setColour(Colours::green);
		g.drawLine(0.0f, 0.0f, 0.0f, (float)getHeight(), 2.0f);

	}

private:


	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayHead)
};