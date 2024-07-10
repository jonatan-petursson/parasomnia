#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VzzzPluginAudioProcessorEditor::VzzzPluginAudioProcessorEditor(VzzzPluginAudioProcessor &p)
    : AudioProcessorEditor(&p),
      processorRef(p),
      leftButton("Left", juce::DrawableButton::ButtonStyle::ImageAboveTextLabel),
      rightButton("Right", juce::DrawableButton::ButtonStyle::ImageAboveTextLabel),
      centerButton("Center", juce::DrawableButton::ButtonStyle::ImageAboveTextLabel)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.

    for (int i = 1; i <= 8; ++i)
    {
        juce::Slider *slider = new juce::Slider(juce::Slider::RotaryVerticalDrag, juce::Slider::NoTextBox);

        slider->setRange(0.0, 127.0, 1.0);
        addAndMakeVisible(sliders.add(slider));

        juce::SliderParameterAttachment *attachment = new juce::SliderParameterAttachment(
            *processorRef.parameters->getParameter(getParamId(1, i)),
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

    setSize(400, 300);
    setResizable(true, true);
}

VzzzPluginAudioProcessorEditor::~VzzzPluginAudioProcessorEditor()
{
    attachments.clear();
    sliders.clear();
}

//==============================================================================
void VzzzPluginAudioProcessorEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
}

void VzzzPluginAudioProcessorEditor::updateSliders()
{
    for (int i = 0; i < 8; ++i)
    {
        // sliders[i]->setValue(processorRef.getParameter(currentPage, i));
    }
}

void VzzzPluginAudioProcessorEditor::buttonClicked(juce::String button)
{
    if (button == "left")
    {
        if (currentPage > 0)
        {
            --currentPage;
            updateSliders();
        }
    }
    else if (button == "right")
    {
        if (currentPage < 8 - 1)
        {
            ++currentPage;
            updateSliders();
        }
    }
    else if (button == "center")
    {
        currentPage = 0;
        updateSliders();
    }
}

void VzzzPluginAudioProcessorEditor::resized()
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
