#pragma once

#include <JuceHeader.h>
#include "ParamInfoProvider.h"
//==============================================================================
class ParasomniaPluginAudioProcessor final : public juce::AudioProcessor, public juce::AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    ParasomniaPluginAudioProcessor();
    ~ParasomniaPluginAudioProcessor() override;

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
    static juce::String getParamName(int page, int param) { return ParamInfoProvider::getParamName(page, param); }

    static juce::String getModSpeedParamName(int page, int param) { return ParamInfoProvider::getPageAbbreviation(page) + " " + ParamInfoProvider::getParamShortLabel(page, param) + " Mod"; }
    static juce::String getModSpeedParamId(int page, int param) { return getParamId(page, param) + "_modSpeed"; }

    static juce::String getAudioFollowerAmplitudeParamName(int page, int param) { return getParamName(page, param) + " Audio Follower Amplitude"; }
    static juce::String getAudioFollowerAmplitudeParamId(int page, int param) { return getParamId(page, param) + "_audioFollowerAmplitude"; }

    static juce::String getAudioFollowerSlewParamName(int page, int param) { return getParamName(page, param) + " Audio Follower Slew"; }
    static juce::String getAudioFollowerSlewParamId(int page, int param) { return getParamId(page, param) + "_audioFollowerSlew"; }

    static juce::String getModAmplitudeParamName(int page, int param) { return getParamName(page, param) + " Modulation Amplitude"; }
    static juce::String getModAmplitudeParamId(int page, int param) { return getParamId(page, param) + "_modAmplitude"; }

    static juce::String getModShapeParamName(int page, int param) { return getParamName(page, param) + " Modulation Shape"; }
    static juce::String getModShapeParamId(int page, int param) { return getParamId(page, param) + "_modShape"; }

    void incrementModulationParameter(const juce::String &paramId, bool increment);
    void incrementRenderScale(bool increment);

    juce::Array<MidiDeviceInfo> getAvailableMidiDevices();
    void changeMidiOutputDevice(const juce::String &deviceIdentifier);

    void updateSmoothing(float smoothing);
    void resetModulation(int page, int param);
    void init();

    void onCenterButtonUp();
    void sendSysExMessage(juce::String message);
    void openPage(int page);

private:
    std::unique_ptr<juce::MidiOutput> midiOutput;
    std::vector<juce::String> paramIds;
    void sendMidiMessage(const juce::MidiMessage &message);
    void parameterChanged(const juce::String &parameterId, float newValue) override;
    void updateStateFromParameters();
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParasomniaPluginAudioProcessor)
};
