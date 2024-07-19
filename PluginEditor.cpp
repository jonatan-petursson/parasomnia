#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PageComponent.h"
#include "ParamInfoProvider.h"
#include "CustomLookAndFeel.h"
#include "JUCE/modules/juce_audio_plugin_client/Standalone/juce_StandaloneFilterWindow.h"

ParasomniaPluginAudioProcessorEditor::ParasomniaPluginAudioProcessorEditor(ParasomniaPluginAudioProcessor &p)
    : AudioProcessorEditor(&p),
      processorRef(p),
      tabController(juce::TabbedButtonBar::TabsAtTop),
      smoothingSlider("Smoothing", *p.parameters->getParameter("smoothing")),
      renderScaleControl("Render Scale", [&p]()
                         { p.incrementRenderScale(false); }, [&p]()
                         { p.incrementRenderScale(true); }),
      initButton("Init", "Init"), globalLabel("Global", "Global"), midiDeviceSelector(p), lookAndFeel()
{
    setLookAndFeel(&lookAndFeel);

    if (juce::JUCEApplicationBase::isStandaloneApp())
    {
        if (auto *pluginHolder = juce::StandalonePluginHolder::getInstance())
        {
            pluginHolder->getMuteInputValue().setValue(false);
        }
    }

    getConstrainer()->setFixedAspectRatio(0.6);

    addAndMakeVisible(globalLabel);
    addAndMakeVisible(smoothingSlider);
    addAndMakeVisible(renderScaleControl);
    addAndMakeVisible(initButton);
    addAndMakeVisible(midiDeviceSelector);

    initButton.onClick = [&p]()
    { p.init(); };

    for (int i = 1; i <= 8; ++i)
    {
        tabController.addTab(ParamInfoProvider::getPageShortTitle(i), juce::Colours::transparentBlack, new PageComponent(processorRef, *this, i), true);
    }

    tabController.setSize(400, 400);
    tabController.setColour(juce::TabbedComponent::ColourIds::outlineColourId, juce::Colours::transparentBlack);
    // tabController.setColour(juce::TabbedComponent::ColourIds::backgroundColourId, juce::Colours::black.withAlpha(0.05f));
    tabController.setTabBarDepth(40);
    tabController.getTabbedButtonBar().setColour(juce::TabbedButtonBar::ColourIds::tabOutlineColourId, juce::Colours::transparentBlack);
    tabController.getTabbedButtonBar().setColour(juce::TabbedButtonBar::ColourIds::frontOutlineColourId, juce::Colours::transparentBlack);

    addAndMakeVisible(tabController);

    setSize(500, 833);
    setResizable(true, true);
}

ParasomniaPluginAudioProcessorEditor::~ParasomniaPluginAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
}

//==============================================================================
void ParasomniaPluginAudioProcessorEditor::paint(juce::Graphics &g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::white);
    g.setFont(15.0f);

    g.setColour(juce::Colours::white.withAlpha(0.1f));
    auto controlsArea = getLocalBounds().removeFromBottom((int)(getHeight() * 0.20));
    g.fillRect(controlsArea.toFloat());
}

void ParasomniaPluginAudioProcessorEditor::updateSliders()
{
}

void ParasomniaPluginAudioProcessorEditor::buttonClicked(juce::String button)
{
    juce::Logger::writeToLog("Button clicked: " + button);
    if (button == "left")
    {
        if (currentPage > 0)
        {
            --currentPage;
        }
    }
    else if (button == "right")
    {
        if (currentPage < 8 - 1)
        {
            ++currentPage;
        }
    }
    else if (button == "center")
    {
        processorRef.onCenterButtonUp();
    }
}

void ParasomniaPluginAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    auto controlsArea = area.removeFromBottom((int)(getHeight() * 0.20));

    tabController.setBounds(area);

    auto controlWidth = controlsArea.getWidth() / 3;

    controlsArea.removeFromTop(10);
    globalLabel.setBounds(controlsArea.removeFromTop(20));
    globalLabel.setJustificationType(juce::Justification::centred);

    auto footerArea = controlsArea.removeFromBottom(30);

    smoothingSlider.setBounds(controlsArea.removeFromLeft(controlWidth).reduced(10));
    renderScaleControl.setBounds(controlsArea.removeFromLeft(controlWidth).reduced(10));
    initButton.setBounds(controlsArea.removeFromLeft(controlWidth).reduced(10));

    midiDeviceSelector.setBounds(footerArea.removeFromLeft(200).reduced(5).withLeft(10));
}