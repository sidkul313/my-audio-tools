# my-audio-tools

A collection of basic audio tools, starting with a sine wave generator plugin built with [JUCE](https://juce.com).

## Sine Generator

A simple synth plugin that plays a pure sine wave.

- **Frequency knob:** 20 Hz to 20,000 Hz, weighted so the low notes get more of the turn. Drag the knob up or down, or click the number and type an exact value.
- **Play button:** starts and stops the tone. It's off when the plugin opens, so it never starts making noise unexpectedly.

It builds as a **VST3** plugin, for use in a DAW, and as a **standalone app**.

## Building on Windows

### What you need

- [Visual Studio 2022 or newer](https://visualstudio.microsoft.com/) (the free Community edition is fine), with the **Desktop development with C++** workload
- [JUCE 9](https://juce.com/get-juce), unzipped to `C:\JUCE`
- [Git](https://git-scm.com/download/win)

### Steps

1. Clone this repository:
   ```
   git clone https://github.com/sidkul313/my-audio-tools
   ```
2. Open `C:\JUCE\Projucer.exe`, choose **File → Open**, and open `Sine-generator/Sine-generator.jucer`.
3. If JUCE isn't in `C:\JUCE`, set its location under **File → Global Paths**.
4. Press **Ctrl+S**, then click the Visual Studio icon at the top to open the project.
5. In Visual Studio's Solution Explorer, right-click **Sine-generator_StandalonePlugin**, choose **Set as Startup Project**, and press **F5**.

The `Builds` and `JuceLibraryCode` folders aren't stored in the repo. Projucer creates them when you save in step 4.

## Project layout

```
Sine-generator/
├── Sine-generator.jucer      Projucer project settings
└── Source/
    ├── PluginProcessor.h/.cpp   the audio: parameters and the sine oscillator
    └── PluginEditor.h/.cpp      the window: frequency knob and Play button
```

## How it works

Each audio sample is `amplitude × sin(phase)`. After every sample, the phase moves forward by `2π × frequency ÷ sampleRate`, and wraps back to 0 after each full cycle. The frequency and play settings are JUCE parameters (`AudioProcessorValueTreeState`), which lets the knob, the button and a DAW's automation all change them safely while audio is playing.

## Planned

- Volume knob
- Smooth fade when starting and stopping, to avoid clicks
- Remember settings when a DAW reopens the plugin
