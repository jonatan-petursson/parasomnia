#pragma once

#include <JuceHeader.h>

class LabeledSlider : public juce::Component
{
public:
    LabeledSlider(const juce::String &labelText);
    void resized() override;

    juce::Slider slider;
    juce::Slider &getSlider() { return slider; }

    juce::Label label;
    juce::Label &getLabel() { return label; }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LabeledSlider)
};
