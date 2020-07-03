/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
WIDEPUTINAudioProcessorEditor::WIDEPUTINAudioProcessorEditor (WIDEPUTINAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1100, 400);

    widthDial.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    widthDial.setRange(WIDTH_MIN, WIDTH_MAX);
    widthDial.setValue(WIDTH_DEFAULT);
    //gainDial.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    widthDial.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    widthDial.setPopupDisplayEnabled(true, false, this);
    widthDial.setSkewFactorFromMidPoint(0.0);
    addAndMakeVisible(&widthDial);

    widthDialAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(processor.parameters, WIDTH_ID, widthDial);

    putin = ImageFileFormat::loadFrom(BinaryData::putinwalkingcropped_jpg, (size_t) BinaryData::putinwalkingcropped_jpgSize);
    putinComponent.setImage(putin);
    putinComponent.setProcessor(&processor);
    addAndMakeVisible(&putinComponent);

    Timer::startTimerHz(60);

}

WIDEPUTINAudioProcessorEditor::~WIDEPUTINAudioProcessorEditor()
{
    Timer::stopTimer();
}

//==============================================================================
void WIDEPUTINAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll(Colour::fromRGB(153, 141, 81));
    g.setColour(Colours::white);

    g.drawText("W I D E", 0, 20, getWidth(), 20, Justification::centredTop);
    //g.drawImage(putin, 100, 160, 400, 400, 0, 0, putin.getWidth(), putin.getHeight());
}

void WIDEPUTINAudioProcessorEditor::resized()
{
    widthDial.setBounds(getWidth() / 2 - 50, 40, 100, 100);
    putinComponent.setBounds(0, 140, getWidth(), 400);
}

void WIDEPUTINAudioProcessorEditor::timerCallback()
{
    putinComponent.repaint();
}
///////////////////////////////////////////////////////////////////////////////////
// PutinComponent
///////////////////////////////////////////////////////////////////////////////////

PutinComponent::PutinComponent()
{

}

void PutinComponent::paint(Graphics& g)
{
    int  width = static_cast<int>(*processor->parameters.getRawParameterValue(WIDTH_ID));
    g.fillAll(Colour::fromRGB(153, 141, 81));
    g.setColour(Colours::white);

    g.drawImage(image, (((getWidth() - 100) / 2) - 5*width) / 2, 0, 600 + (5*width), 400, 0, 0, image.getWidth(), image.getHeight());
}

void PutinComponent::resized()
{
    //this->setBounds(getLocalBounds());
}

void PutinComponent::setImage(Image image)
{
    this->image = image;
}

void PutinComponent::setProcessor(WIDEPUTINAudioProcessor* processor)
{
    this->processor = processor;
}
