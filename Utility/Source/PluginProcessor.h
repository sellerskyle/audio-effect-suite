/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#define GAIN_ID "gain"
#define GAIN_NAME "Gain"
#define GAIN_MAX 12.0f
#define GAIN_MIN -48.0f
#define GAIN_DEFAULT 0.0f

#define PAN_ID "pan"
#define PAN_NAME "Pan"
#define PAN_MAX 100.0f
#define PAN_MIN -100.0f
#define PAN_DEFAULT 0.0f

#define INVERT_LEFT_ID "invert left"
#define INVERT_LEFT_NAME "Invert Left"
#define INVERT_LEFT_DEFAULT false

#define INVERT_RIGHT_ID "invert right"
#define INVERT_RIGHT_NAME "Invert Right"
#define INVERT_RIGHT_DEFAULT false

#define SWAP_CHANNELS_ID "swap channels"
#define SWAP_CHANNELS_NAME "Swap Channels"
#define SWAP_CHANNELS_DEFAULT false

#define MID_SIDE_ID "mid side"
#define MID_SIDE_NAME "Mid Side"
#define MID_SIDE_DEFAULT false


//==============================================================================
/**
*/
class UtilityAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    UtilityAudioProcessor();
    ~UtilityAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    float previousGaindB;
    AudioProcessorValueTreeState parameters; //used for automation and saving state
    AudioProcessorValueTreeState::ParameterLayout createParameters();

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UtilityAudioProcessor)
};
