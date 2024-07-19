#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "ParamInfoProvider.h"
//==============================================================================
ParasomniaPluginAudioProcessor::ParasomniaPluginAudioProcessor()
    : AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth && !JucePlugin_IsStandalone
                         .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
                         .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
      )
{
    std::vector<std::unique_ptr<juce::AudioProcessorParameterGroup>> groups;

    groups.reserve(9);
    paramIds.reserve(200);

    auto globalGroup = std::make_unique<juce::AudioProcessorParameterGroup>("global", "Global", "|");
    globalGroup->addChild(std::make_unique<juce::AudioParameterFloat>("smoothing", "Smoothing", 0.0f, 0.9f, 0.9f));
    paramIds.push_back("smoothing");

    groups.push_back(std::move(globalGroup));

    for (int i = 1; i <= 8; i++)
    {
        auto pageId = juce::String("page_") + juce::String(i);
        auto pageName = juce::String("Page ") + juce::String(i);

        auto group = std::make_unique<juce::AudioProcessorParameterGroup>(
            pageId,
            pageName,
            juce::String("|"));

        for (int j = 1; j <= 8; j++)
        {
            juce::String paramId = getParamId(i, j);
            juce::String paramName = getParamName(i, j);
            group->addChild(std::make_unique<juce::AudioParameterInt>(paramId, paramName, 0, 127, 0));
            paramIds.push_back(paramId);

            juce::String modSpeedParamId = getModSpeedParamId(i, j);
            juce::String modSpeedParamName = getModSpeedParamName(i, j);
            group->addChild(std::make_unique<juce::AudioParameterInt>(modSpeedParamId, modSpeedParamName, 0, 127, 64));
            paramIds.push_back(modSpeedParamId);
        }

        groups.push_back(std::move(group));
    }

    parameters = new juce::AudioProcessorValueTreeState(*this, nullptr, juce::Identifier("parasomnia"), {groups.begin(), groups.end()});

    // create listener for all parameters
    for (auto &paramId : paramIds)
    {
        parameters->addParameterListener(paramId, this);
    }
}

ParasomniaPluginAudioProcessor::~ParasomniaPluginAudioProcessor()
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

void ParasomniaPluginAudioProcessor::incrementModulationParameter(const juce::String &paramId, bool increment)
{
    juce::StringArray parts;
    parts.addTokens(paramId, "_", "\"");

    int page = parts[1].getIntValue();
    int param = parts[3].getIntValue();
    auto modType = parts[4];
    int modTypeNumber = 0;

    if (modType == "audioFollowerAmplitude")
    {
        modTypeNumber = 7;
    }
    else if (modType == "audioFollowerSlew")
    {
        modTypeNumber = 6;
    }
    else if (modType == "modAmplitude")
    {
        modTypeNumber = 1;
    }
    else if (modType == "modShape")
    {
        modTypeNumber = 2;
    }
    else
    {
        juce::Logger::writeToLog("Unknown mod type: " + modType + ", ignoring.");
        return;
    }

    sendSysExMessage("mod_menu_increment,1," +
                     juce::String(param + (8 * (page - 1))) + "," +
                     juce::String(modTypeNumber) + "," +
                     (increment ? "1" : "0") + "," +
                     "1,");
}

void ParasomniaPluginAudioProcessor::incrementRenderScale(bool increment)
{
    sendSysExMessage("render_scale," + juce::String(increment ? "1" : "0") + ",");
}

void ParasomniaPluginAudioProcessor::onCenterButtonUp()
{
    sendSysExMessage("bt_event,2,up,");
}

void ParasomniaPluginAudioProcessor::openPage(int page)
{
    sendSysExMessage("open_page," + juce::String(page - 1) + ",");
}

void ParasomniaPluginAudioProcessor::sendSysExMessage(juce::String message)
{
    std::size_t size = message.getNumBytesAsUTF8();
    auto messageChars = std::make_unique<char[]>(size);
    message.copyToUTF8(messageChars.get(), size);

    juce::Span<std::byte> byteSpan(reinterpret_cast<std::byte *>(messageChars.get()), size);
    juce::MidiMessage sysexMessage = juce::MidiMessage::createSysExMessage(byteSpan);

    if (midiOutput != nullptr)
    {
        juce::Logger::writeToLog("Sending sysex message: " + message + " bytes: " + sysexMessage.getDescription());
        midiOutput->sendMessageNow(sysexMessage);
    }
}

void ParasomniaPluginAudioProcessor::updateSmoothing(float value)
{
    sendSysExMessage("smoothing," + juce::String(value) + ",");
}

void ParasomniaPluginAudioProcessor::resetModulation(int page, int param)
{
    sendSysExMessage("reset," + juce::String(1) + "," + juce::String(((page - 1) * 8) + param) + ",");
}

void ParasomniaPluginAudioProcessor::init()
{
    for (const auto &paramId : paramIds)
    {
        if (auto *param = parameters->getParameter(paramId))
        {
            param->setValueNotifyingHost(param->getDefaultValue());
        }
    }

    sendSysExMessage("init,");
}

void ParasomniaPluginAudioProcessor::parameterChanged(const juce::String &parameterId, float newValue)
{
    if (parameterId == "smoothing")
    {
        updateSmoothing(newValue);
        return;
    }

    juce::StringArray parts;
    parts.addTokens(parameterId, "_", "\"");

    int page = parts[1].getIntValue();
    int param = parts[3].getIntValue();
    int cc = (8 * (page - 1)) + param;

    if (parts.size() == 4)
    {
        juce::Logger::writeToLog("Sending CC " + juce::String(cc) + " value " + juce::String(newValue) + " for parameter " + parameterId);
        sendMidiMessage(juce::MidiMessage::controllerEvent(1, cc, (int)newValue));
    }
    else if (parts.size() == 5 && parts[4] == "modSpeed")
    {
        sendMidiMessage(juce::MidiMessage::controllerEvent(1, cc + 65, (int)newValue));
    }
    else
    {
        juce::Logger::writeToLog("Unknown parameter: " + parameterId);
    }
}

//==============================================================================
const juce::String ParasomniaPluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ParasomniaPluginAudioProcessor::acceptsMidi() const
{
#if JucePlugin_WantsMidiInput
    return true;
#else
    return false;
#endif
}

bool ParasomniaPluginAudioProcessor::producesMidi() const
{
#if JucePlugin_ProducesMidiOutput
    return true;
#else
    return false;
#endif
}

bool ParasomniaPluginAudioProcessor::isMidiEffect() const
{
#if JucePlugin_IsMidiEffect
    return true;
#else
    return false;
#endif
}

double ParasomniaPluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ParasomniaPluginAudioProcessor::getNumPrograms()
{
    return 1; // NB: some hosts don't cope very well if you tell them there are 0 programs,
              // so this should be at least 1, even if you're not really implementing programs.
}

int ParasomniaPluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ParasomniaPluginAudioProcessor::setCurrentProgram(int index)
{
    juce::ignoreUnused(index);
}

const juce::String ParasomniaPluginAudioProcessor::getProgramName(int index)
{
    juce::ignoreUnused(index);
    return {};
}

void ParasomniaPluginAudioProcessor::changeProgramName(int index, const juce::String &newName)
{
    juce::ignoreUnused(index, newName);
}

//==============================================================================
void ParasomniaPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    juce::ignoreUnused(sampleRate, samplesPerBlock);

    if (midiOutput == nullptr)
    {
        auto devs = juce::MidiOutput::getAvailableDevices();
        midiOutput = juce::MidiOutput::openDevice(juce::MidiOutput::getDefaultDevice().identifier);
    }
}

juce::Array<MidiDeviceInfo> ParasomniaPluginAudioProcessor::getAvailableMidiDevices()
{
    return juce::MidiOutput::getAvailableDevices();
}

void ParasomniaPluginAudioProcessor::changeMidiOutputDevice(const juce::String &deviceIdentifier)
{
    if (midiOutput != nullptr)
    {
        if (midiOutput->getDeviceInfo().identifier == deviceIdentifier)
            return;

        juce::Logger::writeToLog("Changing MIDI output device from " + midiOutput->getDeviceInfo().name);

        midiOutput->stopBackgroundThread();
        midiOutput = nullptr;
    }

    juce::Logger::writeToLog("Opening MIDI output device: " + deviceIdentifier);

    midiOutput = juce::MidiOutput::openDevice(deviceIdentifier);
}

void ParasomniaPluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

bool ParasomniaPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout &layouts) const
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
#if !JucePlugin_IsSynth && !JucePlugin_IsStandalone
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
#endif

    return true;
#endif
}

void ParasomniaPluginAudioProcessor::sendMidiMessage(const juce::MidiMessage &message)
{
    if (midiOutput != nullptr)
    {
        juce::Logger::writeToLog("Sending MIDI message: " + juce::String(message.getControllerNumber()) + " " + juce::String(message.getControllerValue()));
        midiOutput->sendMessageNow(message);
    }
}

void ParasomniaPluginAudioProcessor::processBlock(juce::AudioBuffer<float> &buffer,
                                                  juce::MidiBuffer &midiMessages)
{
    juce::ignoreUnused(midiMessages);
    juce::ScopedNoDenormals noDenormals;
    buffer.clear();
}

//==============================================================================
bool ParasomniaPluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor *ParasomniaPluginAudioProcessor::createEditor()
{
    return new ParasomniaPluginAudioProcessorEditor(*this);
}

//==============================================================================
void ParasomniaPluginAudioProcessor::getStateInformation(juce::MemoryBlock &destData)
{
    auto state = parameters->copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void ParasomniaPluginAudioProcessor::setStateInformation(const void *data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters->state.getType()))
            parameters->replaceState(juce::ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor *JUCE_CALLTYPE createPluginFilter()
{
    return new ParasomniaPluginAudioProcessor();
}
