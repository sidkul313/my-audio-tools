/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SinegeneratorAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    SinegeneratorAudioProcessorEditor (SinegeneratorAudioProcessor&);
    ~SinegeneratorAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SinegeneratorAudioProcessor& audioProcessor;
    juce::Slider frequencyKnob;
    juce::Label frequencyLabel;
    juce::TextButton playButton{ "Play" };

    //connecting controls to the processor
    juce::AudioProcessorValueTreeState::SliderAttachment frequencyAttachment;
    juce::AudioProcessorValueTreeState::ButtonAttachment playAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SinegeneratorAudioProcessorEditor)
};
