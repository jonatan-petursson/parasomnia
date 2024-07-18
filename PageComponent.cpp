#include "PageComponent.h"
#include "PluginEditor.h"
#include "ParamInfoProvider.h"

PageComponent::PageComponent(VzzzPluginAudioProcessor &p, VzzzPluginAudioProcessorEditor &pe, int pageArg)
    : processorRef(p),
      modulationTabs(juce::TabbedButtonBar::Orientation::TabsAtTop),
      centerButton("Center", juce::DrawableButton::ButtonStyle::ImageAboveTextLabel),
      pageLabel(ParamInfoProvider::getPageShortTitle(pageArg), ParamInfoProvider::getPageTitle(pageArg)),
      page(pageArg)
{

    pageLabel.setFont(FontOptions(15.0f));
    pageLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(pageLabel);

    for (int i = 1; i <= 8; ++i)
    {
        LabeledSlider *ls = new LabeledSlider(
            ParamInfoProvider::getParamLabel(page, i),
            *p.parameters->getParameter(VzzzPluginAudioProcessor::getParamId(page, i)), [this, i]()
            {
                for(auto lsl : labeledSliders){
                    lsl->setSelected(false);
                }
                labeledSliders[i-1]->setSelected(true);
                modulationTabs.setCurrentTabIndex(i - 1); });

        if (i == 1)
        {
            ls->setSelected(true);
        }

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

    centerButton.setImages(drawableRef, drawableOverRef, nullptr, nullptr, nullptr);

    centerButton.setButtonStyle(juce::DrawableButton::ButtonStyle::ImageFitted);

    centerButton.setButtonText("Center");
    addAndMakeVisible(centerButton);

    centerButton.onClick = [&pe]
    { pe.buttonClicked("center"); };

    modulationTabs.setTabBarDepth(0);
    modulationTabs.setColour(juce::TabbedComponent::ColourIds::backgroundColourId, juce::Colour::fromRGB(71, 71, 181).brighter(0.05f));
    modulationTabs.setColour(juce::TabbedComponent::ColourIds::outlineColourId, juce::Colours::transparentBlack);
    setSize(400, 500);
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

    totalBounds.removeFromTop(10);
    auto headerBounds = totalBounds.removeFromTop(20);
    pageLabel.setBounds(headerBounds);

    // Reserve space for modulationTabs
    int modulationTabsHeight = totalBounds.getHeight() / 4; // 20% of total height
    auto gridBounds = totalBounds.removeFromTop(totalBounds.getHeight() - modulationTabsHeight);

    int minDimension = juce::jmin(gridBounds.getWidth(), gridBounds.getHeight());
    auto gridInnerBounds = juce::Rectangle<int>(0, 0, minDimension, minDimension).withCentre(gridBounds.getCentre());

    // Calculate relative margins
    int relativeMargin = (int)(minDimension * 0.1f); // 10% of the grid size

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
        Gi(labeledSliders[6]).withMargin(Gi::Margin(0, -relativeMargin, 0, relativeMargin)),
        Gi(),
        Gi(centerButton).withMargin(Gi::Margin(relativeMargin * 0.5f)),
        Gi(),
        Gi(labeledSliders[2]).withMargin(Gi::Margin(0, relativeMargin, 0, -relativeMargin)));

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
