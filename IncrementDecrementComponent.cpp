#include "IncrementDecrementComponent.h"

IncrementDecrementComponent::IncrementDecrementComponent(juce::String labelText, std::function<void()> incrementCallback, std::function<void()> decrementCallback)
    : label(labelText, labelText),
      IncrementCallback(std::move(incrementCallback)),
      DecrementCallback(std::move(decrementCallback))
{
    label.setJustificationType(Justification::centred);
    label.setFont(FontOptions(14.0f));
    label.setColour(Label::textColourId, Colours::white);
    addAndMakeVisible(label);

    IncrementButton.setButtonText("+");
    IncrementButton.setTriggeredOnMouseDown(true);
    IncrementButton.onClick = [this]
    {
        IncrementButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(71, 71, 181).darker(0.8f));
        startTimer(50);
        IncrementCallback(); };
    IncrementButton.setRepeatSpeed(500, 200, 100);

    addAndMakeVisible(IncrementButton);

    DecrementButton.setButtonText("-");
    DecrementButton.setTriggeredOnMouseDown(true);
    DecrementButton.onClick = [this]
    {
        DecrementButton.setColour(juce::TextButton::buttonColourId, juce::Colour::fromRGB(71, 71, 181).darker(0.8f));
        startTimer(50);
        DecrementCallback(); };
    DecrementButton.setRepeatSpeed(500, 200, 100);
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

void IncrementDecrementComponent::timerCallback()
{
    IncrementButton.removeColour(juce::TextButton::buttonColourId);
    DecrementButton.removeColour(juce::TextButton::buttonColourId);
}