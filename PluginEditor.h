#pragma once

class PageComponent;

#include "PluginProcessor.h"
#include "LabelTextProvider.h"
//==============================================================================
class VzzzPluginAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit VzzzPluginAudioProcessorEditor(VzzzPluginAudioProcessor &);
    ~VzzzPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

    void updateSliders();
    void buttonClicked(juce::String button);

    static juce::String getParamId(int page, int param) { return VzzzPluginAudioProcessor::getParamId(page, param); }
    static juce::String getParamName(int page, int param) { return VzzzPluginAudioProcessor::getParamName(page, param); }
    static juce::String getParamLabel(int page, int param)
    {
        return LabelTextProvider::getParamLabel(page, param);
    }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VzzzPluginAudioProcessor &processorRef;

    juce::OwnedArray<PageComponent> pages;
    juce::TabbedComponent tabController;

    int currentPage = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VzzzPluginAudioProcessorEditor)
};
