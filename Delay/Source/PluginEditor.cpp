/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayAudioProcessorEditor::DelayAudioProcessorEditor (DelayAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (300, 300);

    delayTimeDial.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    delayTimeDial.setRange(DELAY_TIME_MIN, DELAY_TIME_MAX);
    //delayTimeDial.setSkewFactorFromMidPoint(1000.0);
    delayTimeDial.setValue(DELAY_TIME_DEFAULT);
    delayTimeDial.setTextBoxStyle(Slider::TextBoxBelow, false, 80, 20);
   //delayTimeDial.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&delayTimeDial);

    delayTimeDialAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, DELAY_TIME_ID, delayTimeDial);


}

DelayAudioProcessorEditor::~DelayAudioProcessorEditor()
{
}

//==============================================================================
void DelayAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    //g.setColour (Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);
}

void DelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    delayTimeDial.setBounds(getLocalBounds().reduced(20));
}
