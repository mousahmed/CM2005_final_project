/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer *_player,
                 AudioFormatManager &formatManagerToUse,
                 AudioThumbnailCache &cacheToUse,
                 DJAudioPlayer *_otherPlayer) : player(_player),
                                                waveformDisplay(formatManagerToUse, cacheToUse),
                                                otherPlayer(_otherPlayer)
{

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(loadButton);

    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);

    addAndMakeVisible(waveformDisplay);

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);

    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 100.0);
    posSlider.setRange(0.0, 1.0);
    //===================Modified Code======================================
    // Set tooltips
    playButton.setTooltip("Play the track");
    stopButton.setTooltip("Stop the track");
    loadButton.setTooltip("Load a track");
    volSlider.setTooltip("Adjust volume");
    speedSlider.setTooltip("Adjust speed");
    posSlider.setTooltip("Adjust position");

    // Set button colors
    playButton.setColour(TextButton::buttonColourId, Colours::green);
    stopButton.setColour(TextButton::buttonColourId, Colours::red);
    loadButton.setColour(TextButton::buttonColourId, Colours::blue);

    // Set slider colors
    volSlider.setColour(Slider::thumbColourId, Colours::yellow);
    speedSlider.setColour(Slider::thumbColourId, Colours::orange);
    posSlider.setColour(Slider::thumbColourId, Colours::purple);
    //===================END Modified Code======================================
    addAndMakeVisible(autoFadeButton);
    autoFadeButton.addListener(this);
    autoFadeButton.setTooltip("Auto-fade between tracks");
    autoFadeButton.setColour(TextButton::buttonColourId, Colours::purple);
    startTimer(500);

    addAndMakeVisible(volLabel);
    addAndMakeVisible(speedLabel);
    addAndMakeVisible(posLabel);

    volLabel.setText("Volume", dontSendNotification);
    speedLabel.setText("Speed", dontSendNotification);
    posLabel.setText("Position", dontSendNotification);

    volLabel.attachToComponent(&volSlider, false);
    speedLabel.attachToComponent(&speedSlider, false);
    posLabel.attachToComponent(&posSlider, false);
}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint(Graphics &g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId)); // clear the background

    g.setColour(Colours::grey);
    g.drawRect(getLocalBounds(), 1); // draw an outline around the component

    g.setColour(Colours::white);
    g.setFont(14.0f);
    g.drawText("DeckGUI", getLocalBounds(),
               Justification::centred, true); // draw some placeholder text
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 12; // Adjusted to fit the new button and labels
    playButton.setBounds(0, 0, getWidth(), rowH);
    stopButton.setBounds(0, rowH, getWidth(), rowH);
    autoFadeButton.setBounds(0, rowH * 2, getWidth(), rowH);
    volLabel.setBounds(0, rowH * 3.2, getWidth(), rowH / 2);
    volSlider.setBounds(0, rowH * 3.5, getWidth(), rowH);
    speedLabel.setBounds(0, rowH * 4.7, getWidth(), rowH / 2);
    speedSlider.setBounds(0, rowH * 5, getWidth(), rowH);
    posLabel.setBounds(0, rowH * 6, getWidth(), rowH / 2);
    posSlider.setBounds(0, rowH * 6.7, getWidth(), rowH);
    waveformDisplay.setBounds(0, rowH * 7.5, getWidth(), rowH * 3);
    loadButton.setBounds(0, rowH * 11, getWidth(), rowH);
}

void DeckGUI::buttonClicked(Button *button)
{
    if (button == &playButton)
    {
        std::cout << "Play button was clicked " << std::endl;
        player->start();
        playButton.setColour(TextButton::buttonColourId, Colours::darkgreen);
    }
    if (button == &stopButton)
    {
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();
        stopButton.setColour(TextButton::buttonColourId, Colours::darkred);
    }
    if (button == &loadButton)
    {
        auto fileChooserFlags =
            FileBrowserComponent::canSelectFiles;
        fChooser.launchAsync(fileChooserFlags, [this](const FileChooser &chooser)
                             {
            File chosenFile = chooser.getResult();
            if (chosenFile.exists()){
                player->loadURL(URL{chooser.getResult()});
                waveformDisplay.loadURL(URL{chooser.getResult()});
                loadButton.setColour(TextButton::buttonColourId, Colours::darkblue);
            } });
    }
    if (button == &autoFadeButton)
    {
        std::cout << "Auto-Fade button was clicked " << std::endl;
        // Check if both decks are playing
        if (player->isPlaying() && otherPlayer->isPlaying())
        {
            player->fadeVolume(player->getGain(), 0.0, 3000);
            otherPlayer->fadeVolume(otherPlayer->getGain(), 1.0, 3000);
        }
    }
}

void DeckGUI::sliderValueChanged(Slider *slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }

    if (slider == &posSlider)
    {
        player->setPositionRelative(slider->getValue());
    }
}

bool DeckGUI::isInterestedInFileDrag(const StringArray &files)
{
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}

void DeckGUI::filesDropped(const StringArray &files, int x, int y)
{
    std::cout << "DeckGUI::filesDropped" << std::endl;
    if (files.size() == 1)
    {
        player->loadURL(URL{File{files[0]}});
    }
}

void DeckGUI::timerCallback()
{
    // std::cout << "DeckGUI::timerCallback" << std::endl;
    waveformDisplay.setPositionRelative(
        player->getPositionRelative());
}

void DeckGUI::loadWaveform(URL audioURL)
{
    waveformDisplay.loadURL(audioURL);
}
