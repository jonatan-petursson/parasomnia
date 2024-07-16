#include "ModulationComponent.h"
#include "PageComponent.h"
#include "ParamInfoProvider.h"

ModulationComponent::ModulationComponent(VzzzPluginAudioProcessor &p, int page, int param) : label(ParamInfoProvider::getParamName(page, param),
                                                                                                   ParamInfoProvider::getParamLabel(page, param) + " Modulation"),
                                                                                             modSpeedSlider(
                                                                                                 "Mod Speed",
                                                                                                 *p.parameters->getParameter(VzzzPluginAudioProcessor::getModSpeedParamId(page, param)),
                                                                                                 []() {}),
                                                                                             audioFollowerAmplitude(
                                                                                                 "Audio Foll. Amp.",
                                                                                                 [&p, page, param]()
                                                                                                 { p.incrementModulationParameter(VzzzPluginAudioProcessor::getAudioFollowerAmplitudeParamId(page, param), true); },
                                                                                                 [&p, page, param]()
                                                                                                 { p.incrementModulationParameter(VzzzPluginAudioProcessor::getAudioFollowerAmplitudeParamId(page, param), false); }),
                                                                                             audioFollowerSlew(
                                                                                                 "Audio Foll. Slew",
                                                                                                 [&p, page, param]()
                                                                                                 { p.incrementModulationParameter(VzzzPluginAudioProcessor::getAudioFollowerSlewParamId(page, param), true); },
                                                                                                 [&p, page, param]()
                                                                                                 { p.incrementModulationParameter(VzzzPluginAudioProcessor::getAudioFollowerSlewParamId(page, param), false); }),
                                                                                             modAmplitude(
                                                                                                 "Modulation Amp.",
                                                                                                 [&p, page, param]()
                                                                                                 { p.incrementModulationParameter(VzzzPluginAudioProcessor::getModAmplitudeParamId(page, param), true); },
                                                                                                 [&p, page, param]()
                                                                                                 { p.incrementModulationParameter(VzzzPluginAudioProcessor::getModAmplitudeParamId(page, param), false); }),
                                                                                             modShape(
                                                                                                 "Modulation Shape",
                                                                                                 [&p, page, param]()
                                                                                                 { p.incrementModulationParameter(VzzzPluginAudioProcessor::getModShapeParamId(page, param), true); },
                                                                                                 [&p, page, param]()
                                                                                                 { p.incrementModulationParameter(VzzzPluginAudioProcessor::getModShapeParamId(page, param), false); }),
                                                                                             resetButton("Reset", ParamInfoProvider::getParamName(page, param) + "_reset")

{
    addAndMakeVisible(modSpeedSlider);
    addAndMakeVisible(audioFollowerAmplitude);
    addAndMakeVisible(audioFollowerSlew);
    addAndMakeVisible(modAmplitude);
    addAndMakeVisible(modShape);
    addAndMakeVisible(resetButton);

    resetButton.onClick = [&p, page, param]()
    {
        p.parameters->getParameter(VzzzPluginAudioProcessor::getModSpeedParamId(page, param))->setValueNotifyingHost(0.5f);
        p.resetModulation(page, param);
    };

    label.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(label);
}

ModulationComponent::~ModulationComponent()
{
}

void ModulationComponent::paint(juce::Graphics &g)
{
    g.setFont(15.0f);
}

void ModulationComponent::resized()
{
    juce::Grid grid;

    grid.setGap(juce::Grid::Px(10));

    using Track = juce::Grid::TrackInfo;
    using Fr = juce::Grid::Fr;
    using Gi = juce::GridItem;

    grid.templateRows = {Track(Fr(1)), Track(Fr(4))};
    grid.templateColumns = {Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1)), Track(Fr(1))};

    grid.items.add(Gi(label).withArea(1, 2, 2, 5));
    grid.items.add(Gi(resetButton).withArea(1, 5, 2, 6));
    grid.items.add(Gi(audioFollowerSlew).withArea(2, 1));
    grid.items.add(Gi(audioFollowerAmplitude).withArea(2, 2));
    grid.items.add(Gi(modSpeedSlider).withArea(2, 3));
    grid.items.add(Gi(modAmplitude).withArea(2, 4));
    grid.items.add(Gi(modShape).withArea(2, 5));

    auto bounds = getLocalBounds();
    grid.performLayout(bounds.reduced(10));
}