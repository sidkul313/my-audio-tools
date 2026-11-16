/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SinegeneratorAudioProcessor::SinegeneratorAudioProcessor()
    : AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)),
    apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    frequencyParam = apvts.getRawParameterValue("frequency");
    playParam = apvts.getRawParameterValue("play");
}

juce::AudioProcessorValueTreeState::ParameterLayout SinegeneratorAudioProcessor::createParameterLayout()
{
    juce::NormalisableRange<float> range(20.0f, 20000.0f);
    range.setSkewForCentre(1000.0f); // gives the low notes more of the knob's turn

    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID{ "frequency", 1 }, "Frequency", range, 440.0f));
    layout.add(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID{ "play", 1 }, "Play", false));
    return layout;
}

SinegeneratorAudioProcessor::~SinegeneratorAudioProcessor()
{
}

//==============================================================================
const juce::String SinegeneratorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SinegeneratorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SinegeneratorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SinegeneratorAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SinegeneratorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SinegeneratorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SinegeneratorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SinegeneratorAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SinegeneratorAudioProcessor::getProgramName (int index)
{
    return {};
}

void SinegeneratorAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SinegeneratorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    phase = 0.0;//
}

void SinegeneratorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SinegeneratorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SinegeneratorAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    if (playParam->load() < 0.5f) // play is off
    {
        buffer.clear();           // output silence
        return;                   // and skip the oscillator
    }
    // how far the phase moves each sample
    double phaseStep = juce::MathConstants<double>::twoPi * frequencyParam->load() / currentSampleRate;

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        // the wave's value right now
        float value = amplitude * (float)std::sin(phase);

        for (int channel = 0; channel < totalNumOutputChannels; ++channel)
            buffer.setSample(channel, sample, value);

        // move forward, and wrap back once past one full cycle
        phase += phaseStep;
        if (phase >= juce::MathConstants<double>::twoPi)
            phase -= juce::MathConstants<double>::twoPi;
    }

}

//==============================================================================
bool SinegeneratorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SinegeneratorAudioProcessor::createEditor()
{
    return new SinegeneratorAudioProcessorEditor (*this);
}

//==============================================================================
void SinegeneratorAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SinegeneratorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SinegeneratorAudioProcessor();
}
