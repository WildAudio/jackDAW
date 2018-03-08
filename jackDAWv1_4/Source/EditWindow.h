

#include "../JuceLibraryCode/JuceHeader.h"
#include "AudioTrack.h"
#include "PlayHead.h"

class EditWindow : public AudioAppComponent,
	private Timer, public ChangeListener, public Slider::Listener, public Button::Listener,
	public ApplicationCommandTarget, public ChangeBroadcaster

{
public:
	EditWindow(AudioTransportSource *transportSourcesToUse[4], AudioTransportSource &timerToUse, File *filesToUse[4])
		:
		red(false),
		secondsToDisplay(0.0),
		pixelsPerSecond(0.0),
		state(Stopped),
		playHeadX(0.0),
		//transportSource(transportSourceToUse),
		//transportSource2(transportSourceToUse2),
		//transportSource3(transportSourceToUse3),
		//transportSource4(transportSourceToUse4),
		timer(timerToUse),
		currentState(0)
	{

		addAndMakeVisible(&playButton);

		secondsToDisplay = 8 * 60;
		pixelsPerSecond = 1004 / secondsToDisplay;


		addAndMakeVisible(&displayLabel);
		displayLabel.setColour(Label::backgroundColourId, Colours::black);
		displayLabel.setColour(Label::textColourId, Colours::white);
		ss.str(" ");
		displayLabel.setText(ss.str(), sendNotification);


		addAndMakeVisible(displayLabel2);
		displayLabel2.setColour(Label::backgroundColourId, Colours::black);
		displayLabel2.setColour(Label::textColourId, Colours::white);

		formatManager.registerBasicFormats();
		//customSourceTimer.addChangeListener(this);


		//file2 = "C:\\Users\\User\\Music\\test2.wav";

		//files[0] = new File("C:\\Users\\Administrator\\Music\\DrumsForjackDAW\\Kick.wav");
		//files[1] = new File("C:\\Users\\Administrator\\Music\\DrumsForjackDAW\\Snare.wav");
		//files[2] = new File("C:\\Users\\Administrator\\Music\\DrumsForjackDAW\\OHL.wav");
		//files[3] = new File("C:\\Users\\Administrator\\Music\\DrumsForjackDAW\\OHR.wav");

		for (int i = 0; i <= 3; i++) {
			files[i] = filesToUse[i];
		}

		for (int i = 0; i <= 3; i++) {
			transportSources[i] = transportSourcesToUse[i];
		}

		for (int i = 0; i <= 3; i++) {
			tracks[i] = new AudioTrack(pixelsPerSecond, *files[i], *transportSources[i]);
			addAndMakeVisible(tracks[i]);
			tracks[i]->addChangeListener(this);
		}

		/*tracks[0] = new AudioTrack(pixelsPerSecond, "C:\\Users\\User\\Music\\DrumsforDAW\\AmbOH.wav", transportSource);
		addAndMakeVisible(tracks[0]);
		tracks[0]->addChangeListener(this);*/
		//tracks[1] = new AudioTrack(pixelsPerSecond, "C:\\Users\\User\\Music\\DrumsforDAW\\Snare.wav");

		//setNonBufferingRegionSources(formatManager, readerSource, customSourceTimer, file2, mixer);

		/*AudioFormatReader* readerToUse = formatManager.createReaderFor(file2);
		ScopedPointer<AudioFormatReaderSource> newSource = new AudioFormatReaderSource(readerToUse, true);
		customSourceTimer.setSource(newSource, 0, nullptr, readerToUse->sampleRate);
		readerSource = newSource.release();*/

		/*for (int i = 0; i <= 1; i++) {
		if (tracks[i] != nullptr) {
		addAndMakeVisible(tracks[i]);
		tracks[i]->addChangeListener(this);
		}release
		}*/

		addAndMakeVisible(&playHead);

		startTimer(20);

		//customSourceTimer.setPosition(0.0);
		//customSourceTimer.start();

		setSize(1004, 1000);

	}

	~EditWindow()
	{
		releaseResources();
	}



	ApplicationCommandTarget* getNextCommandTarget() override
	{
		// this will return the next parent component that is an ApplicationCommandTarget (in this
		// case, there probably isn't one, but it's best to use this method in your own apps).
		return findFirstTargetParentComponent();
	}

	void getAllCommands(Array<CommandID>& commands) override
	{
		// this returns the set of all commands that this target can perform..
		CommandID ids[] = {
			begin,
			pause,
			split
		};

		commands.addArray(ids, numElementsInArray(ids));
	}

	void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override
	{
		String generalCategory("General");
		String demosCategory("Demos");

		switch (commandID)
		{
		case 8448:
			result.setInfo("Play", "Plays the Track", demosCategory, 0);
			result.addDefaultKeypress('f', ModifierKeys::commandModifier);
			break;

		case 8449:
			result.setInfo("Pause", "Pauses the track", demosCategory, 0);
			result.addDefaultKeypress('g', ModifierKeys::commandModifier);
			break;

		case 8450:
			result.setInfo("Split", "Pauses the track", demosCategory, 0);
			result.addDefaultKeypress('B', ModifierKeys::commandModifier);
			break;

		default:
			break;
		}
	}

	bool perform(const InvocationInfo& info) override
	{

		switch (info.commandID)
		{
		case 8448:   playButtonClicked(); break;
		case 8449:   stopButtonClicked(); break;

		case 8450:   
			for (int i = 0; i <= 3; i++) {
			if (tracks[i] != nullptr) {
				tracks[i]->breakRegion(playHeadX, timer);
				tracks[i]->repaint();
			}
			}; break;

		default:
			return false;
		}


		return true;
	}

	void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override
	{
		

	}

	void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override
	{
		
	
			bufferToFill.clearActiveBufferRegion();
		
		
	}

	void releaseResources() {
		//customSourceTimer.releaseResources();
		//mixer.releaseResources();
		//delete tracks[0];
		//delete tracks[1];
		for (int i = 0; i <= 3; i++) {
			delete tracks[i];
		}
		/*for (int i = 0; i <= 3; i++) {
			delete files[i];
		}*/
	}

	
	void paint(Graphics& g) override
	{
		/*if (red == true) {
			g.fillAll(Colours::red);
		}
		else {
			g.fillAll(Colours::white);
		}*/
		g.fillAll(Colours::grey);
		for (int i = 0; i <= 3; i++) {

			tracks[i]->setBounds(0, ((i + 1) * 125) - 95, getWidth(), 120);

		}
		displayLabel.setBounds(2, 400, (getWidth() - 4) / 4, 55);
		displayLabel2.setBounds(400, 400, (getWidth() - 4) / 4, 55);
		playHead.setBounds(playHeadX, 0, 5, getHeight());
	}


	void resized() override
	{

	}

	int getPlayHeadX() {
		return playHeadX;
	}

	void changeListenerCallback(ChangeBroadcaster* source) override
	{
		for (int i = 0; i <= 3; i++) {
			if (source == tracks[i]) {
				if (tracks[i]->getActiveRegion() == tracks[i]->getRegionMoved()) {
					tracks[i]->reposition(playHeadX, timer);
				}
			}
		}
	}

	void buttonClicked(Button* button) override
	{

	}

	void timerCallback() override
	{

		playHeadX = (timer.getCurrentPosition() * pixelsPerSecond);
		playHead.repaint();
		if (timer.isPlaying()) {
			for (int i = 0; i <= 3; i++) {
				if (tracks[i] != nullptr) {
					for (int j = 0; j <= tracks[i]->getNumberOfRegions(); j++) {
						if (tracks[i]->getRegions(j) != nullptr) {
							if (playHeadX == tracks[i]->getRegions(j)->getOffset() && currentPlayHeadX != tracks[i]->getRegions(j)->getOffset()) {
								tracks[i]->reposition(playHeadX, timer);
							}
							else if (playHeadX == tracks[i]->getRegions(j)->getXValueEnd() && currentPlayHeadX != tracks[i]->getRegions(j)->getXValueEnd()) {
								tracks[i]->getAudioTransportSource().stop();
							}
						}
					}
				}
			}
			currentPlayHeadX = playHeadX;
		}
	}

	AudioTrack *getTrack(int trackPassed) {
		if (tracks[trackPassed] != nullptr) {
			return tracks[trackPassed];
		}
		else {
			return nullptr;
		}

	}

	int getCurrentState() {
		return currentState;
	}

	void setCurrentState(int stateToUse) {
		currentState = stateToUse;
	}

private:

	void mouseDown(const MouseEvent &e) override
	{
		playHeadX = e.position.x;
		timer.setPosition(playHeadX / pixelsPerSecond);
		//transportSource.setPosition(playHeadX / pixelsPerSecond);

		for (int i = 0; i <= 3; i++) {

			tracks[i]->reposition(playHeadX, timer);

		}

		playHead.repaint();

	}

	

	void sliderValueChanged(Slider* slider) override
	{
		/*if (slider == &slider1) {
		reverbSize = slider1.getValue();
		reverbParameters.roomSize = reverbSize;
		reverbSource.setParameters(reverbParameters);
		}*/
	}

	enum TransportState
	{
		Stopped,
		Paused,
		Playing
	};

	void playButtonClicked()
	{
		if (currentState == 1) {
			currentState = 2;
			sendChangeMessage();
		}
		else {
			currentState = 1;
			sendChangeMessage();
		}
	}

	void stopButtonClicked()
	{
		currentState = 0;
		sendChangeMessage();
	}

	void changeState(TransportState newState)
	{
		if (state != newState)
		{
			state = newState;

			switch (state)
			{
			case Stopped:                           // [3]

													//customSourceTimer.stop();
													//customSourceTimer.setPosition(0.0);
													/*for (int i = 0; i <= 1; i++) {
													if (tracks[i] != nullptr && &tracks[i]->getAudioTransportSource() != nullptr) {*/
				tracks[0]->getAudioTransportSource().stop();
				ss.str("Fuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuuck");
				displayLabel.setText(ss.str(), sendNotification);
				displayLabel.repaint();
				/*}
				else {
				break;
				}
				}*/
				break;

			case Playing:                          // [4]
				timer.start();
				/*for (int i = 0; i <= 1; i++) {
				if (tracks[i] != nullptr && &tracks[i]->getAudioTransportSource() != nullptr) {*/
				tracks[0]->reposition(playHeadX, timer);
				/*}
				else {
				break;
				}
				}*/
				break;

			case Paused:
				timer.stop();
				/*for (int i = 0; i <= 1; i++) {
				if (tracks[i] != nullptr && &tracks[i]->getAudioTransportSource() != nullptr) {*/
				tracks[0]->getAudioTransportSource().stop();
				/*}
				}*/
				break;
			}
		}
	}


	stringstream ss, ss2;
	//int xValue, minutes, seconds, xValue2, minutes2, seconds2, xValue3, activeRegion, widthPassed;
	double secondsToDisplay, pixelsPerSecond;// , secondsToReturn, milliseconds, initialisition;
	int playHeadX, currentPlayHeadX, currentState;
	Label displayLabel, displayLabel2;

	File file1;
	File file2;
	File file3;

	File *files[4];

	//AudioTransportSource customSourceTimer;

	//MixerAudioSource mixer;


	PlayHead playHead;

	TransportState state;

	int samples;
	double rate;

	AudioFormatManager formatManager;
	ScopedPointer<AudioFormatReaderSource> readerSource;

	//bool regionStates[3];

	//bool activeRegionsFound;

	AudioTrack *tracks[4];

	/*AudioTransportSource &transportSource;
	AudioTransportSource &transportSource2;
	AudioTransportSource &transportSource3;
	AudioTransportSource &transportSource4;*/
	AudioTransportSource &timer;

	AudioTransportSource *transportSources[4];


	//==========================================================================

	bool red;

	TextButton playButton;

	CommandID begin = 8448;
	CommandID pause = 8449;
	CommandID split = 8450;


	

};