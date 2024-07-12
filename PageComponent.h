#pragma once

class VzzzPluginAudioProcessorEditor;

#include "PluginProcessor.h"
#include "ModulationComponent.h"

class PageComponent : public juce::Component
{
public:
    PageComponent(VzzzPluginAudioProcessor &p, VzzzPluginAudioProcessorEditor &pe, int pageArg);
    ~PageComponent() override;

    void resized() override;
    void visibilityChanged() override;
    void mouseDown(const juce::MouseEvent &event) override;

private:
    VzzzPluginAudioProcessor &processorRef;
    VzzzPluginAudioProcessorEditor &processorEditorRef;

    juce::OwnedArray<juce::SliderParameterAttachment> attachments;
    juce::OwnedArray<juce::Slider> sliders;

    juce::OwnedArray<ModulationComponent> modulationComponents;

    juce::TabbedComponent modulationTabs;

    juce::DrawableButton leftButton;
    juce::DrawableButton rightButton;
    juce::DrawableButton centerButton;

    int page;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PageComponent)
};
