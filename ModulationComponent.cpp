#include "ModulationComponent.h"
#include "PageComponent.h"

ModulationComponent::ModulationComponent(VzzzPluginAudioProcessor &p, int page, int param)
{
    // Create and add sliders
    for (int i = 1; i <= 5; ++i)
    {
        auto *slider = sliders.add(new juce::Slider(juce::Slider::SliderStyle::RotaryVerticalDrag, juce::Slider::NoTextBox));
        slider->setRange(0.0, 1.0);
        slider->setValue(0.5);
        slider->setNumDecimalPlacesToDisplay(2);

        // Attach the slider to the corresponding parameter
        auto param_id = VzzzPluginAudioProcessor::getParamId(page, param, i);

        attachments.add(
            new juce::SliderParameterAttachment(
                *p.parameters->getParameter(param_id),
                *slider,
                nullptr));

        addAndMakeVisible(slider);
    }
}

ModulationComponent::~ModulationComponent()
{
}

void ModulationComponent::paint(juce::Graphics &g)
{
    g.fillAll(juce::Colours::darkgrey);

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
}

void ModulationComponent::resized()
{
    juce::Grid grid;

    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Gi = juce::GridItem;

    grid.templateRows = {Track(Fr(1))};
    grid.templateColumns = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};

    for (int i = 0; i < sliders.size(); ++i)
    {
        grid.items.add(Gi(*sliders[i]));
    }

    auto bounds = getLocalBounds();
    grid.performLayout(bounds);
}
