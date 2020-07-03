/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
UtilityAudioProcessor::UtilityAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ), parameters(*this, nullptr, "Parameters", createParameters())
#endif
{
}

UtilityAudioProcessor::~UtilityAudioProcessor()
{

    parameters.state = ValueTree("savedParams");
}

//==============================================================================
const String UtilityAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool UtilityAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool UtilityAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool UtilityAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double UtilityAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int UtilityAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int UtilityAudioProcessor::getCurrentProgram()
{
    return 0;
}

void UtilityAudioProcessor::setCurrentProgram (int index)
{
}

const String UtilityAudioProcessor::getProgramName (int index)
{
    return {};
}

void UtilityAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void UtilityAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{

    previousGaindB = *parameters.getRawParameterValue(GAIN_ID);
}

void UtilityAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool UtilityAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void UtilityAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    float currentGaindB = *parameters.getRawParameterValue(GAIN_ID);
    float panVal = (*parameters.getRawParameterValue(PAN_ID)) / 100;

    bool invertLeft = *parameters.getRawParameterValue(INVERT_LEFT_ID);
    float invertLeftVal = invertLeft ? -1.0f : 1.0f;

    bool invertRight = *parameters.getRawParameterValue(INVERT_RIGHT_ID);
    float invertRightVal = invertRight ? -1.0f : 1.0f;

    bool swapChannels = *parameters.getRawParameterValue(SWAP_CHANNELS_ID);

    bool midSide = *parameters.getRawParameterValue(MID_SIDE_ID);

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    AudioBuffer<float> tempBuffer(buffer);

    //MidSide
    if (midSide)
    {
        const float* bufferDataLeft = buffer.getReadPointer(0);
        const float* bufferDataRight = buffer.getReadPointer(1);

        tempBuffer.copyFrom(0, 0, bufferDataLeft, buffer.getNumSamples());
        tempBuffer.copyFrom(1, 0, bufferDataLeft, buffer.getNumSamples());
        tempBuffer.addFrom(0, 0, bufferDataRight, buffer.getNumSamples()); //Left is Mid
        tempBuffer.addFromWithRamp(1, 0, bufferDataRight, buffer.getNumSamples() ,-1.0f, -1.0f); //Right is Side

        buffer = tempBuffer;



    }

    //Channel Swap
    if (swapChannels)
    {
        const float* bufferDataLeft = buffer.getReadPointer(0);
        const float* bufferDataRight = buffer.getReadPointer(1);
        tempBuffer.copyFrom(0, 0, bufferDataRight, buffer.getNumSamples());
        tempBuffer.copyFrom(1, 0, bufferDataLeft, buffer.getNumSamples());

        buffer = tempBuffer;
    }

    //Panning and channel inversion
    auto* leftChannelData = buffer.getWritePointer(0);
    auto* rightChannelData = buffer.getWritePointer(1);

    //AudioBuffer<float> bufferCopy;

    //bufferCopy.copyFrom()

    if (panVal > 0)
    {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto left = *leftChannelData;
            auto right = *rightChannelData;
            *leftChannelData = (left * (1 - panVal)) * invertLeftVal;
            *rightChannelData = (right) + (left * panVal) * invertRightVal;

            ++leftChannelData;
            ++rightChannelData;
        }
    }
    else if (panVal < 0)
    {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto left = *leftChannelData;
            auto right = *rightChannelData;
            *leftChannelData = (left) + (right * (-panVal)) * invertLeftVal;
            *rightChannelData = (right * (1 + panVal)) * invertRightVal;

            ++leftChannelData;
            ++rightChannelData;
        }
    }
    else if (invertLeft || invertRight)
    {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            auto left = *leftChannelData;
            auto right = *rightChannelData;
            *leftChannelData = (left) * invertLeftVal;
            *rightChannelData = (right) * invertRightVal;

            ++leftChannelData;
            ++rightChannelData;
        }
    }



    //Gain
    if (currentGaindB == previousGaindB)
    {
        buffer.applyGain(Decibels::decibelsToGain<float>(currentGaindB, GAIN_MIN));
    }
    else
    {
        buffer.applyGainRamp(0, buffer.getNumSamples(), Decibels::decibelsToGain<float>(previousGaindB, GAIN_MIN), Decibels::decibelsToGain<float>(currentGaindB, GAIN_MIN));
        previousGaindB = currentGaindB;
    }







    //for (int channel = 0; channel < totalNumInputChannels; ++channel)
    //{
    //    auto* channelData = buffer.getWritePointer (channel);

    //    // ..do something to the data...
    //}
}

//==============================================================================
bool UtilityAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* UtilityAudioProcessor::createEditor()
{
    return new UtilityAudioProcessorEditor (*this);
}

//==============================================================================
void UtilityAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    std::unique_ptr<XmlElement> xml(parameters.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void UtilityAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> theParams(getXmlFromBinary(data, sizeInBytes));

    if (theParams != nullptr)
    {
        if (theParams->hasTagName(parameters.state.getType()))
        {
            parameters.state = ValueTree::fromXml(*theParams);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new UtilityAudioProcessor();
}

AudioProcessorValueTreeState::ParameterLayout UtilityAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> parametersArr;

    parametersArr.push_back(std::make_unique<AudioParameterFloat>(PAN_ID, PAN_NAME, PAN_MIN, PAN_MAX, PAN_DEFAULT));
    parametersArr.push_back(std::make_unique<AudioParameterFloat>(GAIN_ID, GAIN_NAME, GAIN_MIN, GAIN_MAX, GAIN_DEFAULT));
    parametersArr.push_back(std::make_unique<AudioParameterBool>(INVERT_LEFT_ID, INVERT_LEFT_NAME, INVERT_LEFT_DEFAULT));
    parametersArr.push_back(std::make_unique<AudioParameterBool>(INVERT_RIGHT_ID, INVERT_RIGHT_NAME, INVERT_RIGHT_DEFAULT));
    parametersArr.push_back(std::make_unique<AudioParameterBool>(SWAP_CHANNELS_ID, SWAP_CHANNELS_NAME, SWAP_CHANNELS_DEFAULT));
    parametersArr.push_back(std::make_unique<AudioParameterBool>(MID_SIDE_ID, MID_SIDE_NAME, MID_SIDE_DEFAULT));

    return { parametersArr.begin(), parametersArr.end() };
}
