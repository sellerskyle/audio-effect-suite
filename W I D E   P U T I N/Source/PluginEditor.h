/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

struct PutinComponent : public Component
{
public:

    PutinComponent();
    void paint(Graphics&) override;

    void resized() override;

    void setImage(Image image);

    void setProcessor(WIDEPUTINAudioProcessor* processor);

private:
    WIDEPUTINAudioProcessor* processor;
    Image image;


};

//==============================================================================
/**
*/
class WIDEPUTINAudioProcessorEditor  : public AudioProcessorEditor,
                                       public Timer
{
public:
    WIDEPUTINAudioProcessorEditor (WIDEPUTINAudioProcessor&);
    ~WIDEPUTINAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    
    void resized() override;

    void timerCallback() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    WIDEPUTINAudioProcessor& processor;

    Slider widthDial;
    Image putin;
    PutinComponent putinComponent;

    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> widthDialAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WIDEPUTINAudioProcessorEditor)
};

