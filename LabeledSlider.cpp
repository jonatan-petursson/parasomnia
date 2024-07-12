
#include "LabeledSlider.h"

LabeledSlider::LabeledSlider(const juce::String &labelText)
{
    // Set up the slider
    slider.setSliderStyle(juce::Slider::LinearHorizontal);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 80, 20);
    addAndMakeVisible(slider);

    // Set up the label
    label.setText(labelText, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}

void LabeledSlider::resized()
{
    auto area = getLocalBounds();

    // Reserve space for the label at the bottom
    auto labelHeight = 20;
    auto labelArea = area.removeFromBottom(labelHeight);

    // Position the slider
    slider.setBounds(area);

    // Position the label
    label.setBounds(labelArea);
}
