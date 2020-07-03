/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FilterAudioProcessor::FilterAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ), treeState(*this, nullptr, "Parameters", createParameters()),
                       iirFilter(dsp::IIR::Coefficients<float>::makeLowPass(44100, 20000.0f, 0.1f)),
                       firFilter(dsp::FilterDesign<float>::designFIRLowpassWindowMethod(20000.f, 44100, 21, dsp::WindowingFunction<float>::hamming))
#endif
{
}

FilterAudioProcessor::~FilterAudioProcessor()
{
    treeState.state = ValueTree("savedParams");
}

//==============================================================================
const String FilterAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FilterAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FilterAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FilterAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FilterAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FilterAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FilterAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FilterAudioProcessor::setCurrentProgram (int index)
{
}

const String FilterAudioProcessor::getProgramName (int index)
{
    return {};
}

void FilterAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void FilterAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback **********************************************************************************************************************************************
    // initialisation that you need..

    lastSampleRate = sampleRate;

    dsp::ProcessSpec spec;
    spec.sampleRate = lastSampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getMainBusNumInputChannels();

    stateVariableFilter.reset();
    iirFilter.reset();
    firFilter.reset();
    updateFilter();
    stateVariableFilter.prepare(spec);
    iirFilter.prepare(spec);
    firFilter.prepare(spec);
}

void FilterAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FilterAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void FilterAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());


    dsp::AudioBlock<float> block(buffer);

    int algorithm = *treeState.getRawParameterValue(ALGORITHM_ID);

    if (algorithm == 0) // SVF
    {
        stateVariableFilter.process(dsp::ProcessContextReplacing<float>(block));
    }
    else if (algorithm == 1) //IIR
    {
        iirFilter.process(dsp::ProcessContextReplacing<float>(block));
    }
    else
    {
        firFilter.process(dsp::ProcessContextReplacing<float>(block));
    }
    updateFilter();
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    //for (int channel = 0; channel < totalNumInputChannels; ++channel)
    //{
    //    auto* channelData = buffer.getWritePointer (channel);

    //    // ..do something to the data...
    //}
}

//==============================================================================
bool FilterAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* FilterAudioProcessor::createEditor()
{
    return new FilterAudioProcessorEditor (*this);
}

//==============================================================================
void FilterAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    std::unique_ptr<XmlElement> xml(treeState.state.createXml());
    copyXmlToBinary(*xml, destData);
}

void FilterAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.

    std::unique_ptr<XmlElement> theParams(getXmlFromBinary(data, sizeInBytes));

    if (theParams != nullptr)
    {
        if (theParams->hasTagName(treeState.state.getType()))
        {
            treeState.state = ValueTree::fromXml(*theParams);
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FilterAudioProcessor();
}

AudioProcessorValueTreeState::ParameterLayout FilterAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<RangedAudioParameter>> parameters;

    parameters.push_back(std::make_unique<AudioParameterChoice>(ALGORITHM_ID, ALGORITHM_NAME, StringArray("SVF", "IIR", "FIR"), 0));
    parameters.push_back(std::make_unique<AudioParameterChoice>(MENU_ID, MENU_NAME, StringArray("Low Pass", "Band Pass", "High Pass"), 0));
    parameters.push_back(std::make_unique<AudioParameterFloat>(CUTOFF_ID, CUTOFF_NAME, CUTOFF_MIN, CUTOFF_MAX, CUTOFF_DEFAULT));
    parameters.push_back(std::make_unique<AudioParameterFloat>(RES_ID, RES_NAME, RES_MIN, RES_MAX, RES_DEFAULT));

    return { parameters.begin(), parameters.end() };
}

void FilterAudioProcessor::updateFilter()
{
    int algorithmChoice = *treeState.getRawParameterValue(ALGORITHM_ID);
    int menuChoice = *treeState.getRawParameterValue(MENU_ID);
    float cutoff = *treeState.getRawParameterValue(CUTOFF_ID);
    float resonance = *treeState.getRawParameterValue(RES_ID);

    if (algorithmChoice == 0) //SVF
    {
        if (menuChoice == 0) //Low Pass
        {
            stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::lowPass;
        }
        else if (menuChoice == 1) //Band Pass
        {
            stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::bandPass;
        }
        else //High Pass
        {
            stateVariableFilter.state->type = dsp::StateVariableFilter::Parameters<float>::Type::highPass;
        }

        stateVariableFilter.state->setCutOffFrequency(lastSampleRate, cutoff, resonance);
        std::cout << "Using SVF";
    }
    else if (algorithmChoice == 1)//IIR
    {
        if (menuChoice == 0) //Low Pass
        {
            *iirFilter.state = *dsp::IIR::Coefficients<float>::makeLowPass(lastSampleRate, cutoff, resonance);
        }
        else if (menuChoice == 1) //Band Pass
        {
            *iirFilter.state = *dsp::IIR::Coefficients<float>::makeBandPass(lastSampleRate, cutoff, resonance);
        }
        else //High Pass
        {
            *iirFilter.state = *dsp::IIR::Coefficients<float>::makeHighPass(lastSampleRate, cutoff, resonance);
        }
        std::cout << "Using IIR";
    }
    else
    {
        *firFilter.state = *dsp::FilterDesign<float>::designFIRLowpassWindowMethod(cutoff, lastSampleRate, 21, dsp::WindowingFunction<float>::hamming);
    }


}
