#pragma once

#include "PluginProcessor.h"
#include "PluginEditor.h"

class KnobComponent : public juce::Component
{
public:
    KnobComponent(VzzzPluginAudioProcessor &p, VzzzPluginAudioProcessorEditor &pe, juce::OwnedArray<juce::SliderParameterAttachment> &attachments, int page)
        : processorRef(p),
          processorEditorRef(pe),
          leftButton("Left", juce::DrawableButton::ButtonStyle::ImageAboveTextLabel),
          rightButton("Right", juce::DrawableButton::ButtonStyle::ImageAboveTextLabel),
          centerButton("Center", juce::DrawableButton::ButtonStyle::ImageAboveTextLabel)
    {
        for (int i = 1; i <= 8; ++i)
        {
            juce::Slider *slider = new juce::Slider(juce::Slider::RotaryVerticalDrag, juce::Slider::NoTextBox);

            slider->setRange(0.0, 127.0, 1.0);
            addAndMakeVisible(sliders.add(slider));

            juce::SliderParameterAttachment *attachment = new juce::SliderParameterAttachment(
                *p.parameters->getParameter(VzzzPluginAudioProcessor::getParamId(page, i)),
                *slider,
                nullptr);

            attachments.add(attachment);
        }

        leftButton.setButtonText("Left");
        addAndMakeVisible(leftButton);
        rightButton.setButtonText("Right");
        addAndMakeVisible(rightButton);
        centerButton.setButtonText("Center");
        addAndMakeVisible(centerButton);

        leftButton.onClick = [this, &pe]
        { pe.buttonClicked("left"); };
        rightButton.onClick = [this, &pe]
        { pe.buttonClicked("right"); };
        centerButton.onClick = [this, &pe]
        { pe.buttonClicked("center"); };
    }

    void resized() override
    {
        juce::Grid grid;

        using Track = juce::Grid::TrackInfo;
        using Gi = juce::GridItem;
        using Fr = juce::Grid::Fr;

        grid.templateColumns = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};
        grid.templateRows = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};

        grid.items.add(
            Gi(),
            Gi(),
            Gi(sliders[4]).withMargin(juce::GridItem::Margin(0, 0, -100.0f, 0)),
            Gi(),
            Gi());

        grid.items.add(
            Gi(leftButton),
            Gi(sliders[5]),
            Gi(),
            Gi(sliders[3]),
            Gi(rightButton));

        grid.items.add(
            Gi(sliders[6]).withMargin(juce::GridItem::Margin(0, -100.0f, 0, 0)),
            Gi(),
            Gi(centerButton),
            Gi(),
            Gi(sliders[2]).withMargin(juce::GridItem::Margin(0, 0, 0, -100.0f)));

        grid.items.add(
            Gi(),
            Gi(sliders[7]),
            Gi(),
            Gi(sliders[1]),
            Gi());

        grid.items.add(
            Gi(),
            Gi(),
            Gi(sliders[0]).withMargin(juce::GridItem::Margin(-100.0f, 0, 0, 0)),
            Gi(),
            Gi());

        grid.performLayout(getLocalBounds());
    }

private:
    VzzzPluginAudioProcessor &processorRef;
    VzzzPluginAudioProcessorEditor &processorEditorRef;

    juce::OwnedArray<juce::Slider> sliders;
    juce::DrawableButton leftButton;
    juce::DrawableButton rightButton;
    juce::DrawableButton centerButton;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KnobComponent)
};