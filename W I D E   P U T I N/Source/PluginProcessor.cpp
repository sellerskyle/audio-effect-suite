/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WIDEPUTINAudioProcessor::WIDEPUTINAudioProcessor()
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
    parameters.state = ValueTree("savedParams");
}

WIDEPUTINAudioProcessor::~WIDEPUTINAudioProcessor()
{
}

//==============================================================================
const String WIDEPUTINAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool WIDEPUTINAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool WIDEPUTINAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool WIDEPUTINAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double WIDEPUTINAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int WIDEPUTINAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int WIDEPUTINAudioProcessor::getCurrentProgram()
{
    return 0;
}

void WIDEPUTINAudioProcessor::setCurrentProgram (int index)
{
}

const String WIDEPUTINAudioProcessor::getProgramName (int index)
{
    return {};
}

void WIDEPUTINAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void WIDEPUTINAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    midSideBuffer.setSize(getTotalNumInputChannels(), samplesPerBlock, false, true, true);
    //lastSampleRate = sampleRate;
}

void WIDEPUTINAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool WIDEPUTINAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void WIDEPUTINAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if (totalNumInputChannels != 2) //Only works on strictly stereo data
    {
        return;
    }

    float width = *parameters.getRawParameterValue(WIDTH_ID);

    midSideBuffer.clear();

    const auto* bufferReadLeft = buffer.getReadPointer(0);
    const auto* bufferReadRight = buffer.getReadPointer(1);

    //Mid
    midSideBuffer.copyFrom(0, 0, bufferReadLeft, buffer.getNumSamples());
    midSideBuffer.addFrom(0, 0, bufferReadRight, buffer.getNumSamples());

    //Side
    midSideBuffer.copyFrom(1, 0, bufferReadLeft, buffer.getNumSamples());
    midSideBuffer.addFromWithRamp(1, 0, bufferReadRight, buffer.getNumSamples(), -1.0f, -1.0f);

    //Now to add mid and side to left and right channel


    //Left
    auto* bufferWriteLeft = buffer.getWritePointer(0);
    auto* bufferWriteRight = buffer.getWritePointer(1);

    auto* midRead = midSideBuffer.getReadPointer(0);
    auto* sideRead = midSideBuffer.getReadPointer(1);

    //float midWeight = 1.0f - ((width + 100.0f) / 200.0f);
    float sideWeight = Decibels::decibelsToGain<float>((width / 4), WIDTH_MIN / 4);
    float scaleFactor = (1 + (0.5 * width)) / 100.0f;

    for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        bufferWriteLeft[sample] = (midRead[sample] * 0.5f) + (sideRead[sample] * sideWeight);
        bufferWriteRight[sample] = (midRead[sample] * 0.5f) - (sideRead[sample] * sideWeight);
    }
}

//==============================================================================
bool WIDEPUTINAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* WIDEPUTINAudioProcessor::createEditor()
{
    return new WIDEPUTINAudioProcessorEditor (*this);
}

//==============================================================================
void WIDEPUTINAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    std::unique_ptr<XmlElement> xml(parameters.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void WIDEPUTINAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
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
    return new WIDEPUTINAudioProcessor();
}

AudioProcessorValueTreeState::ParameterLayout  WIDEPUTINAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> parametersArr;

    parametersArr.push_back(std::make_unique<AudioParameterFloat>(WIDTH_ID, WIDTH_NAME, WIDTH_MIN, WIDTH_MAX, WIDTH_DEFAULT));

    return { parametersArr.begin(), parametersArr.end() };
}
