#pragma once

class ParasomniaPluginAudioProcessorEditor;

#include "PluginProcessor.h"
#include "ModulationComponent.h"
#include "LabeledSlider.h"

class PageComponent : public juce::Component
{
public:
    PageComponent(ParasomniaPluginAudioProcessor &p, ParasomniaPluginAudioProcessorEditor &pe, int pageArg);
    ~PageComponent() override;

    void resized() override;
    void visibilityChanged() override;

private:
    ParasomniaPluginAudioProcessor &processorRef;

    juce::OwnedArray<juce::SliderParameterAttachment> attachments;
    juce::OwnedArray<LabeledSlider> labeledSliders;
    juce::OwnedArray<juce::Slider> sliders;
    juce::OwnedArray<juce::DrawableRectangle> debugRectangles;

    juce::OwnedArray<ModulationComponent> modulationComponents;

    juce::TabbedComponent modulationTabs;

    juce::DrawableButton centerButton;

    juce::Label pageLabel;

    int page;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PageComponent)
};