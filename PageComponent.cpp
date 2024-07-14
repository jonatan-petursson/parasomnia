#include "PageComponent.h"
#include "PluginEditor.h"
#include "ParamInfoProvider.h"

PageComponent::PageComponent(VzzzPluginAudioProcessor &p, VzzzPluginAudioProcessorEditor &pe, int pageArg)
    : processorRef(p),
      processorEditorRef(pe),
      leftButton("Left", juce::DrawableButton::ButtonStyle::ImageAboveTextLabel),
      rightButton("Right", juce::DrawableButton::ButtonStyle::ImageAboveTextLabel),
      centerButton("Center", juce::DrawableButton::ButtonStyle::ImageAboveTextLabel),
      modulationTabs(juce::TabbedButtonBar::Orientation::TabsAtTop),
      page(pageArg),
      pageLabel(ParamInfoProvider::getPageShortTitle(pageArg), ParamInfoProvider::getPageTitle(pageArg))
{

    pageLabel.setFont(juce::Font(15.0f));
    pageLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(pageLabel);

    for (int i = 1; i <= 8; ++i)
    {
        LabeledSlider *ls = new LabeledSlider(
            ParamInfoProvider::getParamShortLabel(page, i),
            *p.parameters->getParameter(VzzzPluginAudioProcessor::getParamId(page, i)), [this, i]()
            { modulationTabs.setCurrentTabIndex(i - 1); });

        addAndMakeVisible(ls);
        labeledSliders.add(ls);

        auto modulationHeading = ParamInfoProvider::getParamShortLabel(page, i);
        if (modulationHeading == "")
        {
            modulationHeading = "Unused";
        }

        modulationTabs.addTab(
            modulationHeading,
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
    drawable.setFill(juce::Colours::black.withAlpha(0.2f));

    juce::DrawablePath drawableOver = juce::DrawablePath();
    drawableOver.setPath(path);
    drawableOver.setFill(juce::Colours::black.withAlpha(0.4f));

    const juce::Drawable *drawableRef = &drawable;
    const juce::Drawable *drawableOverRef = &drawableOver;

    leftButton.setImages(drawableRef, drawableOverRef, nullptr, nullptr, nullptr);
    rightButton.setImages(drawableRef, drawableOverRef, nullptr, nullptr, nullptr);
    centerButton.setImages(drawableRef, drawableOverRef, nullptr, nullptr, nullptr);

    centerButton.setButtonStyle(juce::DrawableButton::ButtonStyle::ImageFitted);

    leftButton.setButtonText("Left");
    // addAndMakeVisible(leftButton);
    rightButton.setButtonText("Right");
    // addAndMakeVisible(rightButton);
    centerButton.setButtonText("Center");
    addAndMakeVisible(centerButton);

    leftButton.onClick = [&pe]
    { pe.buttonClicked("left"); };
    rightButton.onClick = [&pe]
    { pe.buttonClicked("right"); };
    centerButton.onClick = [&pe]
    { pe.buttonClicked("center"); };

    modulationTabs.setTabBarDepth(0);
    modulationTabs.setColour(juce::TabbedComponent::ColourIds::outlineColourId, juce::Colours::transparentBlack);
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
    grid.templateRows = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};

    auto totalBounds = getLocalBounds();

    totalBounds.removeFromTop(20);
    auto headerBounds = totalBounds.removeFromTop(20);
    pageLabel.setBounds(headerBounds);

    // Reserve space for modulationTabs
    int modulationTabsHeight = totalBounds.getHeight() / 4; // 20% of total height
    auto gridBounds = totalBounds.removeFromTop(totalBounds.getHeight() - modulationTabsHeight);

    int minDimension = juce::jmin(gridBounds.getWidth(), gridBounds.getHeight());
    auto gridInnerBounds = juce::Rectangle<int>(0, 0, minDimension, minDimension).withCentre(gridBounds.getCentre());

    // Calculate relative margins
    float relativeMargin = minDimension * 0.1f; // 10% of the grid size

    gridInnerBounds.expand(relativeMargin, relativeMargin);

    grid.items.add(
        Gi(),
        Gi(),
        Gi(labeledSliders[4]).withMargin(Gi::Margin(relativeMargin, 0, -relativeMargin, 0)),
        Gi(),
        Gi());

    grid.items.add(
        Gi(),
        Gi(labeledSliders[5]),
        Gi(),
        Gi(labeledSliders[3]),
        Gi());

    grid.items.add(
        Gi(labeledSliders[6]).withMargin(Gi::Margin(0, -relativeMargin * 2, 0, 0)),
        Gi(),
        Gi(centerButton).withMargin(Gi::Margin(relativeMargin * 0.5f)),
        Gi(),
        Gi(labeledSliders[2]).withMargin(Gi::Margin(0, 0, 0, -relativeMargin * 2)));

    grid.items.add(
        Gi(),
        Gi(labeledSliders[7]),
        Gi(),
        Gi(labeledSliders[1]),
        Gi());

    grid.items.add(
        Gi(),
        Gi(),
        Gi(labeledSliders[0]).withMargin(Gi::Margin(-relativeMargin, 0, relativeMargin, 0)),
        Gi(),
        Gi());

    grid.performLayout(gridInnerBounds.reduced(10));

    modulationTabs.setBounds(totalBounds);
}
