/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class UtilityAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    UtilityAudioProcessorEditor (UtilityAudioProcessor&);
    ~UtilityAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    UtilityAudioProcessor& processor;


    Slider panSlider;
    Slider gainDial;
    TextButton invertLeftButton;
    TextButton invertRightButton;
    TextButton swapChannelsButton;
    TextButton midSideButton;
    
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> panSliderAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainDialAttachment;
    std::unique_ptr< AudioProcessorValueTreeState::ButtonAttachment> invertLeftButtonAttachment;
    std::unique_ptr< AudioProcessorValueTreeState::ButtonAttachment> invertRightButtonAttachment;
    std::unique_ptr< AudioProcessorValueTreeState::ButtonAttachment> swapChannelsButtonAttachment;
    std::unique_ptr< AudioProcessorValueTreeState::ButtonAttachment> midSideButtonAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (UtilityAudioProcessorEditor)
};
