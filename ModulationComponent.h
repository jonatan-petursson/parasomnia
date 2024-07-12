#pragma once

#include "PluginProcessor.h"

class ModulationComponent : public juce::Component
{
public:
    ModulationComponent(VzzzPluginAudioProcessor &p, int page, int param);
    ~ModulationComponent() override;

    void paint(juce::Graphics &g) override;
    void resized() override;

private:
    juce::OwnedArray<juce::Slider> sliders;
    juce::OwnedArray<juce::SliderParameterAttachment> attachments;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationComponent)
};
