#pragma once

#include "PluginProcessor.h"

//==============================================================================
class VzzzPluginAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit VzzzPluginAudioProcessorEditor (VzzzPluginAudioProcessor&);
    ~VzzzPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VzzzPluginAudioProcessor& processorRef;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VzzzPluginAudioProcessorEditor)
};
