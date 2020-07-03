/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FilterAudioProcessorEditor::FilterAudioProcessorEditor (FilterAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (200, 300);

    filterAlgorithm.setJustificationType(Justification::centred);
    filterAlgorithm.addItem("SVF", 1);
    filterAlgorithm.addItem("IIR", 2);
    filterAlgorithm.addItem("FIR", 3);
    addAndMakeVisible(&filterAlgorithm);

    algorithmAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.treeState, ALGORITHM_ID, filterAlgorithm);

    filterMenu.setJustificationType(Justification::centred);
    filterMenu.addItem("Low Pass", 1);
    filterMenu.addItem("Band Pass", 2);
    filterMenu.addItem("High Pass", 3);
    addAndMakeVisible(&filterMenu);

    menuAttachment = std::make_unique<AudioProcessorValueTreeState::ComboBoxAttachment>(processor.treeState, MENU_ID, filterMenu);

    filterCutoffDial.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    filterCutoffDial.setRange(CUTOFF_MIN, CUTOFF_MAX);
    filterCutoffDial.setValue(CUTOFF_DEFAULT);
    filterCutoffDial.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    filterCutoffDial.setPopupDisplayEnabled(true, true, this);
    filterCutoffDial.setSkewFactorFromMidPoint(1000.0);
    addAndMakeVisible(&filterCutoffDial);

    cutoffDialAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, CUTOFF_ID, filterCutoffDial);

    filterResDial.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    filterResDial.setRange(RES_MIN, RES_MAX);
    filterResDial.setValue(RES_DEFAULT);
    filterResDial.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    filterResDial.setPopupDisplayEnabled(true, true, this);
    addAndMakeVisible(&filterResDial);

    resDialAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.treeState, RES_ID, filterResDial);



}

FilterAudioProcessorEditor::~FilterAudioProcessorEditor()
{
}

//==============================================================================
void FilterAudioProcessorEditor::paint (Graphics& g)
{
    //// (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    //g.setColour (Colours::white);
    //g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), Justification::centred, 1);

        //background ui stuff
    Rectangle<int> algorithmLabel(0, 40, getWidth(), 20);
    Rectangle<int> titleArea(0, 100, getWidth(), 20);

    g.fillAll(Colours::black);
    g.setColour(Colours::white);
    g.drawText("Algorithm", algorithmLabel, Justification::centredTop);
    g.drawText("Filter", titleArea, Justification::centredTop);
    g.drawText("Cutoff", 46, 170, 50, 25, Justification::centredLeft);
    g.drawText("Resonance", 107, 170, 70, 25, Justification::centredLeft);

    Rectangle <float> area(25, 25, 150, 250);

    g.setColour(Colours::yellow);
    g.drawRoundedRectangle(area, 20.0f, 2.0f);


}

void FilterAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    //need to come back and dynamically set these...ok for now
    Rectangle<int> area = getLocalBounds().reduced(40);

    filterAlgorithm.setBounds(40, 60, getWidth() - 80, 20);
    filterMenu.setBounds(40, 120, getWidth() - 80, 20);
    filterCutoffDial.setBounds(30, 190, 70, 70);
    filterResDial.setBounds(100, 190, 70, 70);
}
