#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PageComponent.h"
#include "LabelTextProvider.h"

PageComponent::PageComponent(VzzzPluginAudioProcessor &p, VzzzPluginAudioProcessorEditor &pe, int pageArg)
    : processorRef(p),
      processorEditorRef(pe),
      leftButton("Left", juce::DrawableButton::ButtonStyle::ImageAboveTextLabel),
      rightButton("Right", juce::DrawableButton::ButtonStyle::ImageAboveTextLabel),
      centerButton("Center", juce::DrawableButton::ButtonStyle::ImageAboveTextLabel),
      modulationTabs(juce::TabbedButtonBar::Orientation::TabsAtTop),
      page(pageArg)
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

        slider->addMouseListener(this, false);

        attachments.add(attachment);

        modulationTabs.addTab(
            juce::String(i),
            juce::Colours::transparentBlack,
            new ModulationComponent(p, page, i),
            true);
    }

    addAndMakeVisible(modulationTabs);

    // add a shape to the buttons
    juce::Path path;
    path.addEllipse(0, 0, 50, 50);

    juce::DrawablePath drawable = juce::DrawablePath();
    drawable.setPath(path);
    drawable.setFill(juce::Colours::grey);

    const juce::Drawable *drawableRef = &drawable;
    // convert to drawable pointer

    leftButton.setImages(drawableRef, nullptr, nullptr, nullptr, nullptr);
    rightButton.setImages(drawableRef, nullptr, nullptr, nullptr, nullptr);
    centerButton.setImages(drawableRef, nullptr, nullptr, nullptr, nullptr);

    centerButton.setButtonStyle(juce::DrawableButton::ButtonStyle::ImageFitted);

    leftButton.setButtonText("Left");
    addAndMakeVisible(leftButton);
    rightButton.setButtonText("Right");
    addAndMakeVisible(rightButton);
    centerButton.setButtonText("Center");
    addAndMakeVisible(centerButton);

    leftButton.onClick = [&pe]
    { pe.buttonClicked("left"); };
    rightButton.onClick = [&pe]
    { pe.buttonClicked("right"); };
    centerButton.onClick = [&pe]
    { pe.buttonClicked("center"); };

    setSize(400, 500);
}

void PageComponent::mouseDown(const juce::MouseEvent &event)
{
    juce::Logger::writeToLog("Mouse down event: x=" + juce::String(event.x) + ", y=" + juce::String(event.y));

    if (event.mods.isLeftButtonDown())
    {
        juce::Slider *slider = dynamic_cast<juce::Slider *>(event.eventComponent);
    }
}

PageComponent::~PageComponent()
{
    attachments.clear();
    sliders.clear();
}

void PageComponent::visibilityChanged()
{
    if (isVisible())
    {
        processorRef.openPage(page);
    }
}

void PageComponent::resized()
{
    juce::Grid grid;

    using Track = juce::Grid::TrackInfo;
    using Gi = juce::GridItem;
    using Fr = juce::Grid::Fr;

    grid.templateColumns = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};
    grid.templateRows = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};

    grid.items.add(
        Gi(leftButton),
        Gi(),
        Gi(sliders[4]).withMargin(Gi::Margin(0, 0, -100.0f, 0)).withJustifySelf(Gi::JustifySelf::center),
        Gi(),
        Gi(rightButton));

    grid.items.add(
        Gi(),
        Gi(sliders[5]),
        Gi(),
        Gi(sliders[3]),
        Gi());

    grid.items.add(
        Gi(sliders[6]).withMargin(Gi::Margin(0, -100.0f, 0, 0)),
        Gi(),
        Gi(centerButton).withMargin(Gi::Margin(10)),
        Gi(),
        Gi(sliders[2]).withMargin(Gi::Margin(0, 0, 0, -100.0f)));

    grid.items.add(
        Gi(),
        Gi(sliders[7]),
        Gi(),
        Gi(sliders[1]),
        Gi());

    grid.items.add(
        Gi(),
        Gi(),
        Gi(sliders[0]).withMargin(Gi::Margin(-100.0f, 0, 0, 0)),
        Gi(),
        Gi());

    grid.items.add(
        Gi(modulationTabs).withMargin(0).withArea(6, 1, 8, 6));

    grid.performLayout(getLocalBounds());
}
