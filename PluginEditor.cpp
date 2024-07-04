#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VzzzPluginAudioProcessorEditor::VzzzPluginAudioProcessorEditor (VzzzPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processorRef (p)
{
    juce::ignoreUnused (processorRef);
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

VzzzPluginAudioProcessorEditor::~VzzzPluginAudioProcessorEditor()
{
}

//==============================================================================
void VzzzPluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("Hello World 2!", getLocalBounds(), juce::Justification::centred, 1);
}

void VzzzPluginAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    juce::FlexBox fb;
    
    fb.flexWrap = juce::FlexBox::Wrap::wrap;
    fb.justifyContent = juce::FlexBox::JustifyContent::center;
    fb.alignContent = juce::FlexBox::AlignContent::center;

    fb.performLayout(getLocalBounds());
}
