
#include "LabeledSlider.h"

LabeledSlider::LabeledSlider(
    const juce::String &labelText,
    juce::RangedAudioParameter &parameter,
    std::function<void()> onClickHandler) : label(labelText, labelText),
                                            attachment(std::make_unique<juce::SliderParameterAttachment>(parameter, slider)),
                                            onClick(onClickHandler)
{
    this->addMouseListener(this, true);

    slider.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::NoTextBox, true, 0, 0);
    addAndMakeVisible(slider);

    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}

void LabeledSlider::mouseUp(const juce::MouseEvent &event)
{
    if (event.mods.isLeftButtonDown())
    {
        onClick();
    }
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
