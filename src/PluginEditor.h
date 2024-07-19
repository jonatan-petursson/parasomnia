#pragma once

class PageComponent;

#include "PluginProcessor.h"
#include "ParamInfoProvider.h"
#include "IncrementDecrementComponent.h"
#include "LabeledSlider.h"
#include "CustomLookAndFeel.h"
#include "MidiDeviceSelector.h"
//==============================================================================
class ParasomniaPluginAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit ParasomniaPluginAudioProcessorEditor(ParasomniaPluginAudioProcessor &);
    ~ParasomniaPluginAudioProcessorEditor() override;

    //==============================================================================
    void paint(juce::Graphics &) override;
    void resized() override;

    void updateSliders();
    void buttonClicked(juce::String button);

    static juce::String getParamId(int page, int param) { return ParasomniaPluginAudioProcessor::getParamId(page, param); }
    static juce::String getParamName(int page, int param) { return ParasomniaPluginAudioProcessor::getParamName(page, param); }
    static juce::String getParamLabel(int page, int param)
    {
        return ParamInfoProvider::getParamLabel(page, param);
    }

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    ParasomniaPluginAudioProcessor &processorRef;

    juce::OwnedArray<PageComponent> pages;
    juce::TabbedComponent tabController;

    LabeledSlider smoothingSlider;
    IncrementDecrementComponent renderScaleControl;
    juce::TextButton initButton;
    juce::Label globalLabel;
    MidiDeviceSelector midiDeviceSelector;

    CustomLookAndFeel lookAndFeel;

    int currentPage = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParasomniaPluginAudioProcessorEditor)
};
