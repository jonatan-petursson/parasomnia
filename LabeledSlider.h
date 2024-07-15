#pragma once

#include <JuceHeader.h>

class LabeledSlider : public juce::Component
{
public:
    LabeledSlider(const juce::String &labelText, juce::RangedAudioParameter &parameter, std::function<void()> onClickHandler = []() {});

    void resized() override;

    juce::Slider slider;
    juce::Slider &getSlider() { return slider; }

    juce::Label label;
    juce::Label &getLabel() { return label; }

    void mouseUp(const juce::MouseEvent &event) override;

private:
    std::unique_ptr<juce::SliderParameterAttachment> attachment;
    std::function<void()> onClick;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LabeledSlider)
};
