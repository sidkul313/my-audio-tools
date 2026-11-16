/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class SinegeneratorAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    SinegeneratorAudioProcessor();
    ~SinegeneratorAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState apvts;
private:
    //==============================================================================
    double currentSampleRate = 44100.0; // samples per second, set properly in prepareToPlay
    double phase = 0.0;                 // where we are in the wave, from 0 to 2π
    std::atomic<float>* frequencyParam = nullptr;
    std::atomic<float>* playParam = nullptr;
    float amplitude = 0.2f;             // volume from 0 to 1, kept low to protect your ears
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SinegeneratorAudioProcessor)
};
