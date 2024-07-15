#pragma once

#include <JuceHeader.h>

class CustomLookAndFeel : public juce::LookAndFeel_V4
{
public:
    void drawButtonBackground(juce::Graphics &g, juce::Button &button, const juce::Colour &backgroundColour,
                              bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override
    {
        juce::ignoreUnused(backgroundColour);

        auto bounds = button.getLocalBounds().toFloat();
        auto baseColour = juce::Colours::black.withAlpha(0.2f);

        if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
            baseColour = baseColour.withAlpha(0.3f);

        g.setColour(baseColour);
        g.fillRoundedRectangle(bounds, 4.0f);
    }

    int getTabButtonSpaceAroundImage() override { return 2; }

    void drawTabButton(juce::TabBarButton &button, juce::Graphics &g, bool isMouseOver, bool isMouseDown) override
    {
        const auto area = button.getActiveArea().reduced(4);
        auto c = juce::Colours::black.withAlpha(0.2f);

        juce::Path path;
        path.addRoundedRectangle(area.getX(), area.getY(), area.getWidth(), area.getHeight(),
                                 6.0f, 6.0f, true, true, true, true);

        if (button.isFrontTab())
        {
            c = c.withAlpha(0.4f);
        }

        g.setColour(c);
        g.fillPath(path);

        drawTabButtonText(button, g, isMouseOver, isMouseDown);
    }

    Font getTabButtonFont(TabBarButton &b, float height) override
    {
        juce::ignoreUnused(height);

        return withDefaultMetrics(FontOptions{15.0f, b.isFrontTab() ? Font::bold : Font::plain});
    }

    CustomLookAndFeel()
    {
        setColourScheme(juce::LookAndFeel_V4::getDarkColourScheme());
    }
};
