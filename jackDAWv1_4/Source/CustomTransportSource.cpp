
#include "../JuceLibraryCode/JuceHeader.h"
#include "CustomAudioTransportSource.h"




/*
==============================================================================

This file is part of the JUCE library.
Copyright (c) 2017 - ROLI Ltd.

JUCE is an open source library subject to commercial or open-source
licensing.

The code included in this file is provided under the terms of the ISC license
http://www.isc.org/downloads/software-support-policy/isc-license. Permission
To use, copy, modify, and/or distribute this software for any purpose with or
without fee is hereby granted provided that the above copyright notice and
this permission notice appear in all copies.

JUCE IS PROVIDED "AS IS" WITHOUT ANY WARRANTY, AND ALL WARRANTIES, WHETHER
EXPRESSED OR IMPLIED, INCLUDING MERCHANTABILITY AND FITNESS FOR PURPOSE, ARE
DISCLAIMED.

==============================================================================
*/

namespace juce
{

	CustomAudioTransportSource::CustomAudioTransportSource(AudioSource* const inputSource, const bool deleteInputWhenDeleted)
		: input(inputSource, deleteInputWhenDeleted),
		bypass(false),
		panL(0.0),
		panR(0.0)
	{
		jassert(inputSource != nullptr);
	}

	//float panL;
	//float panR;

	CustomAudioTransportSource::~CustomAudioTransportSource() {}

	void CustomAudioTransportSource::setPanL(float panLToUse) {
		panL = panLToUse;
	}

	void CustomAudioTransportSource::setPanR(float panRToUse) {
		panR = panRToUse;
	}

	void CustomAudioTransportSource::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
	{
		const ScopedLock sl(lock);
		input->prepareToPlay(samplesPerBlockExpected, sampleRate);
		reverb.setSampleRate(sampleRate);
	}

	void CustomAudioTransportSource::releaseResources() {}

	void CustomAudioTransportSource::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill)
	{
		const ScopedLock sl(lock);

		input->getNextAudioBlock(bufferToFill);

		if (!bypass)
		{

			//float* const buffer = bufferToFill.buffer->getWritePointer(channel, bufferToFill.startSample);

			bufferToFill.buffer->applyGain(0, 0, bufferToFill.buffer->getNumSamples(), 1.0 + panL);
			bufferToFill.buffer->applyGain(1, 0, bufferToFill.buffer->getNumSamples(), 1.0 - panL);

		}
	}

	/*void CustomAudioTransportSource::setParameters(const Reverb::Parameters& newParams)
	{
	const ScopedLock sl(lock);
	reverb.setParameters(newParams);
	}

	void CustomAudioTransportSource::setBypassed(bool b) noexcept
	{
	if (bypass != b)
	{
	const ScopedLock sl(lock);
	bypass = b;
	reverb.reset();
	}
	}*/

} // namespace juce
