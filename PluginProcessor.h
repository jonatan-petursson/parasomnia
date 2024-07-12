#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_audio_devices/midi_io/juce_MidiDevices.h>
#include "LabelTextProvider.h"
//==============================================================================
class VzzzPluginAudioProcessor final : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    VzzzPluginAudioProcessor();
    ~VzzzPluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout &layouts) const override;

    void processBlock(juce::AudioBuffer<float> &, juce::MidiBuffer &) override;
    using AudioProcessor::processBlock;

    //==============================================================================
    juce::AudioProcessorEditor *createEditor() override;
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
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String &newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock &destData) override;
    void setStateInformation(const void *data, int sizeInBytes) override;
    juce::AudioProcessorValueTreeState *parameters;

    static juce::String getParamId(int page, int param) { return "page_" + juce::String(page) + "_param_" + juce::String(param); }
    static juce::String getParamName(int page, int param) { return LabelTextProvider::getParamName(page, param); }

    static juce::String getParamId(int page, int param, int mod) { return "page_" + juce::String(page) + "_param_" + juce::String(param) + "_mod_" + juce::String(mod); }
    static juce::String getParamName(int page, int param, int mod) { return "Page " + juce::String(page) + " Param " + juce::String(param) + " Mod " + juce::String(mod); }

    void onCenterButtonUp();
    void sendSysExMessage(juce::String message);
    void openPage(int page);

private:
    std::unique_ptr<juce::MidiOutput> midiOutput;
    std::vector<juce::String> paramIds;
    void sendMidiMessage(const juce::MidiMessage &message);
    void parameterChanged(const juce::String &parameterId, float newValue) override;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VzzzPluginAudioProcessor)
};
