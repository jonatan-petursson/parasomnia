#pragma once

#include <JuceHeader.h>

class IncrementDecrementComponent : public juce::Component
{
public:
    IncrementDecrementComponent(juce::String labelText, std::function<void()> incrementCallback, std::function<void()> decrementCallback);

    void resized() override;

private:
    juce::Label label;
    juce::TextButton IncrementButton;
    juce::TextButton DecrementButton;

    juce::TextButton IncrementMultiButton;
    juce::TextButton DecrementMultiButton;

    std::function<void()> IncrementCallback;
    std::function<void()> DecrementCallback;
};
