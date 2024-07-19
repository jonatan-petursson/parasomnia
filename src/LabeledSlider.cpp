
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

void LabeledSlider::mouseDown(const juce::MouseEvent &event)
{
    if (event.mods.isLeftButtonDown())
    {
        onClick();
    }
}

void LabeledSlider::setSelected(bool selected)
{
    this->isSelected = selected;
    this->repaint();
}

void LabeledSlider::paint(juce::Graphics &g)
{
    if (isSelected)
    {
        g.setColour(juce::Colour::fromRGB(71, 71, 181).brighter(0.05f));
        g.fillRoundedRectangle(getLocalBounds().toFloat(), 6.0f);
    }

    juce::Component::paint(g);
}

void LabeledSlider::resized()
{
    auto area = getLocalBounds();

    // Reserve space for the label at the bottom
    auto labelHeight = 30;
    auto labelArea = area.removeFromBottom(labelHeight);

    // Position the slider
    slider.setBounds(area);

    // Position the label
    labelArea.removeFromBottom(10);
    label.setBounds(labelArea);
}
