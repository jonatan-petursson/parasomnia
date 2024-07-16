#pragma once

class PageComponent;

#include "PluginProcessor.h"
#include "ParamInfoProvider.h"
#include "IncrementDecrementComponent.h"
#include "LabeledSlider.h"
#include "CustomLookAndFeel.h"
#include "MidiDeviceSelector.h"
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
        return ParamInfoProvider::getParamLabel(page, param);
    }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VzzzPluginAudioProcessor &processorRef;

    juce::OwnedArray<PageComponent> pages;
    juce::TabbedComponent tabController;

    LabeledSlider smoothingSlider;
    IncrementDecrementComponent renderScaleControl;
    juce::TextButton initButton;
    juce::Label globalLabel;
    MidiDeviceSelector midiDeviceSelector;

    CustomLookAndFeel lookAndFeel;

    int currentPage = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(VzzzPluginAudioProcessorEditor)
};
