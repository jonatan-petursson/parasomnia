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
        auto baseColour = button.findColour(juce::TextButton::buttonColourId);

        if (shouldDrawButtonAsDown || shouldDrawButtonAsHighlighted)
            baseColour = baseColour.withAlpha(0.7f);

        g.setColour(baseColour);
        g.fillRoundedRectangle(bounds, 4.0f);
    }

    int getTabButtonSpaceAroundImage() override { return 2; }
    void drawTabAreaBehindFrontButton(TabbedButtonBar &bar, Graphics &g, const int w, const int h) override
    {
        juce::ignoreUnused(bar, g, w, h);
    }
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

    void drawRotarySlider(juce::Graphics &g, int x, int y, int width, int height, float sliderPos,
                          const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider &slider) override
    {
        juce::ignoreUnused(slider);

        auto bounds = juce::Rectangle<int>(x, y, width, height).toFloat().reduced(10);
        auto radius = juce::jmin(bounds.getWidth(), bounds.getHeight()) / 2.0f;
        auto toAngle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);
        auto lineW = juce::jmin(8.0f, radius * 0.5f);
        auto arcRadius = radius - lineW * 0.5f;

        juce::Path backgroundArc;
        backgroundArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), arcRadius, arcRadius, 0.0f,
                                    rotaryStartAngle, rotaryEndAngle, true);

        g.setColour(juce::Colour::fromRGB(71, 71, 181).darker(0.2f));
        g.strokePath(backgroundArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

        juce::Path valueArc;
        valueArc.addCentredArc(bounds.getCentreX(), bounds.getCentreY(), arcRadius, arcRadius, 0.0f,
                               rotaryStartAngle, toAngle, true);

        g.setColour(juce::Colour::fromRGB(71, 71, 181).darker(0.5f)); // Change this color to your desired knob color
        g.strokePath(valueArc, juce::PathStrokeType(lineW, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));
    }

    CustomLookAndFeel()
    {
        auto cs = juce::LookAndFeel_V4::getDarkColourScheme();
        cs.setUIColour(juce::LookAndFeel_V4::ColourScheme::windowBackground, juce::Colour::fromRGB(71, 71, 181));
        cs.setUIColour(juce::LookAndFeel_V4::ColourScheme::widgetBackground, juce::Colour::fromRGB(71, 71, 181).darker(0.2f));

        setColourScheme(cs);
    }
};
