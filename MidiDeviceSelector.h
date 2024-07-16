#pragma once

#include <JuceHeader.h>

class MidiDeviceSelector : public juce::Component,
                           public juce::ComboBox::Listener
{
public:
    MidiDeviceSelector(VzzzPluginAudioProcessor &p);
    ~MidiDeviceSelector() override;

    void resized() override;

    void comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged) override;

    juce::String getSelectedDeviceName() const;

private:
    VzzzPluginAudioProcessor &processorRef;
    juce::ComboBox midiDeviceDropdown;
    std::vector<juce::MidiDeviceInfo> midiDevices;

    juce::String getDeviceIdentifierFromDropdownId(int dropdownId);
    void updateDeviceList();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiDeviceSelector)
};
