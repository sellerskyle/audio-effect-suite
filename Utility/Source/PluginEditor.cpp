/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
UtilityAudioProcessorEditor::UtilityAudioProcessorEditor (UtilityAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (200, 500);

    panSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    panSlider.setRange(PAN_MIN, PAN_MAX);
    panSlider.setValue(PAN_DEFAULT);
    //panSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 120, 25);
    panSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    panSlider.setPopupDisplayEnabled(true, false, this);
    addAndMakeVisible(&panSlider);

    panSliderAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters, PAN_ID, panSlider);

    gainDial.setSliderStyle(Slider::SliderStyle::RotaryHorizontalDrag);
    gainDial.setRange(GAIN_MIN, GAIN_MAX);
    gainDial.setValue(GAIN_DEFAULT);
    //gainDial.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    gainDial.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    gainDial.setPopupDisplayEnabled(true, false, this);
    gainDial.setSkewFactorFromMidPoint(0.0);
    addAndMakeVisible(&gainDial);

    gainDialAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters, GAIN_ID, gainDial);

    invertLeftButton.setColour(TextButton::buttonColourId, Colours::black);
    invertLeftButton.setColour(TextButton::buttonOnColourId, Colours::deepskyblue);
    invertLeftButton.setClickingTogglesState(true);
    invertLeftButton.onClick = [this] {};
    addAndMakeVisible(&invertLeftButton);

    invertLeftButtonAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(processor.parameters, INVERT_LEFT_ID, invertLeftButton);

    invertRightButton.setColour(TextButton::buttonColourId, Colours::black);
    invertRightButton.setColour(TextButton::buttonOnColourId, Colours::deepskyblue);
    invertRightButton.setClickingTogglesState(true);
    invertRightButton.onClick = [this] {};
    addAndMakeVisible(&invertRightButton);

    invertRightButtonAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(processor.parameters, INVERT_RIGHT_ID, invertRightButton);

    swapChannelsButton.setColour(TextButton::buttonColourId, Colours::black);
    swapChannelsButton.setColour(TextButton::buttonOnColourId, Colours::deepskyblue);
    swapChannelsButton.setClickingTogglesState(true);
    swapChannelsButton.onClick = [this] {};
    addAndMakeVisible(&swapChannelsButton);

    swapChannelsButtonAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(processor.parameters, SWAP_CHANNELS_ID, swapChannelsButton);

    midSideButton.setColour(TextButton::buttonColourId, Colours::black);
    midSideButton.setColour(TextButton::buttonOnColourId, Colours::deepskyblue);
    midSideButton.setClickingTogglesState(true);
    midSideButton.onClick = [this] {};
    addAndMakeVisible(&midSideButton);

    midSideButtonAttachment = std::make_unique<AudioProcessorValueTreeState::ButtonAttachment>(processor.parameters, MID_SIDE_ID, midSideButton);


}

UtilityAudioProcessorEditor::~UtilityAudioProcessorEditor()
{
}

//==============================================================================
void UtilityAudioProcessorEditor::paint(Graphics& g)
{
    g.fillAll(Colours::darksalmon);
    g.setColour(Colours::white);

    g.drawText("Pan", 0, 20, getWidth(), 20, Justification::centredTop);
    g.drawText("Gain", 0, 100, getWidth(), 20, Justification::centredTop);

    g.drawText("Left Invert", 0, 260, getWidth()/2, 20, Justification::centredTop);
    g.drawText("Right Invert", getWidth() / 2, 260, getWidth()/2, 20, Justification::centredTop);
    g.drawText("Swap Channels", 0, 340, getWidth(), 20, Justification::centredTop);
    g.drawText("Mid Side", 0, 420, getWidth(), 20, Justification::centredTop);
}


void UtilityAudioProcessorEditor::resized()
{

    panSlider.setBounds(40, 40, getWidth() - 80, 45);
    gainDial.setBounds(40, 120, getWidth() - 80, getWidth() - 80);
    invertLeftButton.setBounds(30, 280, 40, 40);
    invertRightButton.setBounds((getWidth() / 2) + 30, 280, 40, 40);
    swapChannelsButton.setBounds(80, 360, 40, 40);
    midSideButton.setBounds(80, 440, 40, 40);
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
