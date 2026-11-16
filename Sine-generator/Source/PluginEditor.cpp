/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SinegeneratorAudioProcessorEditor::SinegeneratorAudioProcessorEditor(SinegeneratorAudioProcessor& p)
    : AudioProcessorEditor(&p), audioProcessor(p),
    frequencyAttachment(p.apvts, "frequency", frequencyKnob),
    playAttachment(p.apvts, "play", playButton)
{
    frequencyKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    frequencyKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 90, 22);
    frequencyKnob.setNumDecimalPlacesToDisplay(0);
    frequencyKnob.setTextValueSuffix(" Hz");
    addAndMakeVisible(frequencyKnob);

    frequencyLabel.setText("Frequency", juce::dontSendNotification);
    frequencyLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(frequencyLabel);

    playButton.setClickingTogglesState(true); // click on, click off
    playButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::green);
    addAndMakeVisible(playButton);

    setSize(400, 300);
}

SinegeneratorAudioProcessorEditor::~SinegeneratorAudioProcessorEditor()
{
}

//==============================================================================
void SinegeneratorAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void SinegeneratorAudioProcessorEditor::resized()
{
    auto area = getLocalBounds().reduced(20);          // whole window, minus a 20px margin
    playButton.setBounds(area.removeFromBottom(40));  // button: bottom 40px
    area.removeFromBottom(10);                         // 10px gap
    frequencyLabel.setBounds(area.removeFromTop(24)); // label: top 24px
    frequencyKnob.setBounds(area);                     // knob: everything left
}
