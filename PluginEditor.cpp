#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PageComponent.h"
#include "ParamInfoProvider.h"
#include "CustomLookAndFeel.h"
#include "JUCE/modules/juce_audio_plugin_client/Standalone/juce_StandaloneFilterWindow.h"

VzzzPluginAudioProcessorEditor::VzzzPluginAudioProcessorEditor(VzzzPluginAudioProcessor &p)
    : AudioProcessorEditor(&p),
      processorRef(p),
      tabController(juce::TabbedButtonBar::TabsAtTop),
      smoothingSlider("Smoothing", *p.parameters->getParameter("smoothing")),
      renderScaleControl("Render Scale", [&p]()
                         { p.incrementRenderScale(false); }, [&p]()
                         { p.incrementRenderScale(true); }),
      initButton("Init", "Init"), globalLabel("Global", "Global"), lookAndFeel()
{
    setLookAndFeel(&lookAndFeel);

    if (juce::JUCEApplicationBase::isStandaloneApp())
    {
        if (auto *pluginHolder = juce::StandalonePluginHolder::getInstance())
        {
            pluginHolder->getMuteInputValue().setValue(false);
        }
    }

    getConstrainer()->setFixedAspectRatio(0.7);

    addAndMakeVisible(globalLabel);
    addAndMakeVisible(smoothingSlider);
    addAndMakeVisible(renderScaleControl);
    addAndMakeVisible(initButton);

    initButton.onClick = [&p]()
    { p.init(); };

    for (int i = 1; i <= 8; ++i)
    {
        tabController.addTab(ParamInfoProvider::getPageShortTitle(i), juce::Colours::transparentBlack, new PageComponent(processorRef, *this, i), true);
    }

    tabController.setSize(400, 400);
    tabController.setColour(juce::TabbedComponent::ColourIds::outlineColourId, juce::Colours::transparentBlack);
    tabController.setTabBarDepth(40);
    tabController.getTabbedButtonBar().setColour(juce::TabbedButtonBar::ColourIds::tabOutlineColourId, juce::Colours::transparentBlack);
    tabController.getTabbedButtonBar().setColour(juce::TabbedButtonBar::ColourIds::frontOutlineColourId, juce::Colours::transparentBlack);

    addAndMakeVisible(tabController);

    setSize(500, 700);
    setResizable(true, true);
}

VzzzPluginAudioProcessorEditor::~VzzzPluginAudioProcessorEditor()
{
    setLookAndFeel(nullptr);
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
}

void VzzzPluginAudioProcessorEditor::buttonClicked(juce::String button)
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

void VzzzPluginAudioProcessorEditor::resized()
{
    auto area = getLocalBounds();
    auto controlsArea = area.removeFromBottom((int)(getHeight() * 0.15));

    tabController.setBounds(area);

    auto controlWidth = controlsArea.getWidth() / 3;

    globalLabel.setBounds(controlsArea.removeFromTop(20));
    globalLabel.setJustificationType(juce::Justification::centred);

    smoothingSlider.setBounds(controlsArea.removeFromLeft(controlWidth).reduced(10));
    renderScaleControl.setBounds(controlsArea.removeFromLeft(controlWidth).reduced(10));
    initButton.setBounds(controlsArea.removeFromLeft(controlWidth).reduced(10));
}