#pragma once

#include "PluginProcessor.h"
#include "LabeledSlider.h"
#include "IncrementDecrementComponent.h"

class ModulationComponent : public juce::Component
{
public:
    ModulationComponent(VzzzPluginAudioProcessor &p, int page, int param);
    ~ModulationComponent() override;

    void paint(juce::Graphics &g) override;
    void resized() override;

private:
    juce::Label label;
    LabeledSlider modSpeedSlider;
    IncrementDecrementComponent audioFollowerAmplitude;
    IncrementDecrementComponent audioFollowerSlew;
    IncrementDecrementComponent modAmplitude;
    IncrementDecrementComponent modShape;
    juce::TextButton resetButton;
    juce::OwnedArray<juce::SliderParameterAttachment> attachments;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ModulationComponent)
};