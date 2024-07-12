#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "PageComponent.h"
#include "LabelTextProvider.h"

//==============================================================================
VzzzPluginAudioProcessorEditor::VzzzPluginAudioProcessorEditor(VzzzPluginAudioProcessor &p)
    : AudioProcessorEditor(&p),
      processorRef(p),
      tabController(juce::TabbedButtonBar::TabsAtTop)
{
    for (int i = 1; i <= 8; ++i)
    {
        tabController.addTab(LabelTextProvider::getPageShortTitle(i), juce::Colours::transparentBlack, new PageComponent(processorRef, *this, i), true);
    }

    tabController.setSize(400, 400);
    addAndMakeVisible(tabController);

    setSize(400, 400);
    setResizable(true, true);
}

VzzzPluginAudioProcessorEditor::~VzzzPluginAudioProcessorEditor()
{
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
    tabController.setBounds(getLocalBounds());
}
