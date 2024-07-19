#include "PluginProcessor.h"
#include "MidiDeviceSelector.h"

MidiDeviceSelector::MidiDeviceSelector(ParasomniaPluginAudioProcessor &p) : processorRef(p)
{
    addAndMakeVisible(midiDeviceDropdown);
    midiDeviceDropdown.setTextWhenNothingSelected("Use alternate MIDI Device");
    midiDeviceDropdown.addListener(this);

    updateDeviceList();
}

MidiDeviceSelector::~MidiDeviceSelector()
{
    midiDeviceDropdown.removeListener(this);
}

void MidiDeviceSelector::resized()
{
    midiDeviceDropdown.setBounds(getLocalBounds());
}

void MidiDeviceSelector::comboBoxChanged(juce::ComboBox *comboBoxThatHasChanged)
{
    if (comboBoxThatHasChanged == &midiDeviceDropdown)
    {
        juce::String selectedDevice = midiDeviceDropdown.getText();
        juce::Logger::writeToLog("Selecting MIDI device: " + selectedDevice);
        processorRef.changeMidiOutputDevice(getDeviceIdentifierFromDropdownId(midiDeviceDropdown.getSelectedId()));
    }
}

juce::String MidiDeviceSelector::getSelectedDeviceName() const
{
    return midiDeviceDropdown.getText();
}

void MidiDeviceSelector::updateDeviceList()
{
    midiDeviceDropdown.clear();

    midiDevices.clear();

    auto newMidiDevices = processorRef.getAvailableMidiDevices();

    int id = 1;
    for (auto &input : newMidiDevices)
    {
        midiDevices.push_back(input);
        midiDeviceDropdown.addItem(input.name, id);
        ++id;
    }
}

juce::String MidiDeviceSelector::getDeviceIdentifierFromDropdownId(int dropdownId)
{
    if (dropdownId <= 0)
    {
        throw std::out_of_range("Invalid dropdown ID");
    }

    return midiDevices[static_cast<size_t>(dropdownId) - 1].identifier;
}