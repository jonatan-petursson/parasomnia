#include "PluginProcessor.h"
#include "PluginEditor.h"
//==============================================================================
VzzzPluginAudioProcessor::VzzzPluginAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      )
{
    // prepare parameters

    // Create 8 groups of juce::AudioParameters in a loop

    std::vector<std::unique_ptr<juce::AudioProcessorParameterGroup>> groups;
    groups.reserve(8);

    paramIds.reserve(64);

    for (int i = 1; i <= 8; i++)
    {
        auto group = std::make_unique<juce::AudioProcessorParameterGroup>(
            juce::String("page ") + juce::String(i),
            juce::String("page") + juce::String(i),
            juce::String("|"));

        for (int j = 1; j <= 8; j++)
        {
            juce::String paramId = getParamId(i, j);
            group->addChild(std::make_unique<juce::AudioParameterInt>(paramId, getParamName(i, j), 0, 127, 0));
            paramIds.push_back(paramId);
        }

        groups.push_back(std::move(group));
    }

    parameters = new juce::AudioProcessorValueTreeState(*this, nullptr, juce::Identifier("vzzz"), {groups.begin(), groups.end()});

    // create listener for all parameters
    for (auto &paramId : paramIds)
    {
        parameters->addParameterListener(paramId, this);
    }
}

VzzzPluginAudioProcessor::~VzzzPluginAudioProcessor()
{

    if (parameters != nullptr)
    {
        for (auto &paramId : paramIds)
            parameters->removeParameterListener(paramId, this);

        delete parameters;
    }
    if (midiOutput != nullptr)
        midiOutput.reset();
}

void VzzzPluginAudioProcessor::parameterChanged(const juce::String &parameterId, float newValue)
{

    juce::ignoreUnused(parameterId, newValue);
    std::cout << "Parameter changed: " << parameterId << ", new value: " << newValue << std::endl;

    juce::StringArray parts;
    parts.addTokens(parameterId, "_", "\"");

    int page = parts[1].getIntValue();
    int param = parts[3].getIntValue();

    sendMidiMessage(juce::MidiMessage::controllerEvent(1, page * param, newValue));
}

//==============================================================================
const juce::String VzzzPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VzzzPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool VzzzPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool VzzzPluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double VzzzPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VzzzPluginAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int VzzzPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VzzzPluginAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String VzzzPluginAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void VzzzPluginAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void VzzzPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused(sampleRate, samplesPerBlock);

    if (midiOutput == nullptr)
        midiOutput = juce::MidiOutput::openDevice(juce::MidiOutput::getDefaultDevice().identifier);
}

void VzzzPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool VzzzPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
{
#if JucePlugin_IsMidiEffect
    juce::ignoreUnused(layouts);
    return true;
#else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

        // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

void VzzzPluginAudioProcessor::sendMidiMessage(const juce::MidiMessage &message)
{
    if (midiOutput != nullptr)
    {
        juce::Logger::writeToLog("MIDI Message Sent: " + message.getDescription());
        midiOutput->sendMessageNow(message);
    }
}

void VzzzPluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                            juce::MidiBuffer &midiMessages)
{

    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto *channelData = buffer.getWritePointer(channel);
        juce::ignoreUnused(channelData);
        // ..do something to the data...
    }
}

//==============================================================================
bool VzzzPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *VzzzPluginAudioProcessor::createEditor()
{
    return new VzzzPluginAudioProcessorEditor(*this);
}

//==============================================================================
void VzzzPluginAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    juce::ignoreUnused(destData);
}

void VzzzPluginAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    juce::ignoreUnused(data, sizeInBytes);
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new VzzzPluginAudioProcessor();
}
