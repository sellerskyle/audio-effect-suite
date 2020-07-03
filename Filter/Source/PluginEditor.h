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
class FilterAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    FilterAudioProcessorEditor (FilterAudioProcessor&);
    ~FilterAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FilterAudioProcessor& processor;

    ComboBox filterAlgorithm;
    ComboBox filterMenu;
    Slider filterCutoffDial;
    Slider filterResDial;

    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> algorithmAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::ComboBoxAttachment> menuAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> cutoffDialAttachment;
    std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> resDialAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilterAudioProcessorEditor)
};
