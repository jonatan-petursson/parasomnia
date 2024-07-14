#include "IncrementDecrementComponent.h"

IncrementDecrementComponent::IncrementDecrementComponent(juce::String labelText, std::function<void()> incrementCallback, std::function<void()> decrementCallback)
    : IncrementCallback(std::move(incrementCallback)), DecrementCallback(std::move(decrementCallback)), label(labelText, labelText)
{
    label.setJustificationType(Justification::centred);
    label.setFont(Font(14.0f));
    label.setColour(Label::textColourId, Colours::white);
    addAndMakeVisible(label);

    IncrementButton.setButtonText("+");
    IncrementButton.onClick = [this]
    { IncrementCallback(); };
    IncrementButton.setRepeatSpeed(300, 100);
    addAndMakeVisible(IncrementButton);

    DecrementButton.setButtonText("-");
    DecrementButton.onClick = [this]
    { DecrementCallback(); };
    DecrementButton.setRepeatSpeed(300, 100);
    addAndMakeVisible(DecrementButton);
}

void IncrementDecrementComponent::resized()
{
    auto area = getLocalBounds();
    auto labelHeight = 20;
    auto buttonHeight = (area.getHeight() - labelHeight) / 2;

    IncrementButton.setBounds(area.removeFromTop(buttonHeight));
    area.removeFromTop(2);
    DecrementButton.setBounds(area.removeFromTop(buttonHeight));
    label.setBounds(area);
}
