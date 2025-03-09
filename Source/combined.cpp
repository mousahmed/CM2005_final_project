// START CustomLookAndFeel.h
/*
  ==============================================================================

    CustomLookAndFeel.h
    Created: 08 Mar 2025 12:19:32pm
    Author:  MousA

  ==============================================================================
*/
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class CustomLookAndFeel : public LookAndFeel_V4
{
public:
  CustomLookAndFeel()
  {
    setColour(Slider::thumbColourId, Colours::yellow);
    setColour(Slider::trackColourId, Colours::grey);
  }

  void drawLinearSliderThumb(Graphics &g, int x, int y, int width, int height, float sliderPos,
                             float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider &slider) override
  {
    g.setColour(findColour(Slider::thumbColourId));
    g.fillRect(Rectangle<float>(static_cast<float>(x), sliderPos - 5.0f, static_cast<float>(width), 10.0f));
  }

  void drawLinearSlider(Graphics &g, int x, int y, int width, int height, float sliderPos,
                        float minSliderPos, float maxSliderPos, const Slider::SliderStyle style, Slider &slider) override
  {
    g.setColour(findColour(Slider::trackColourId));
    g.fillRect(Rectangle<float>(static_cast<float>(x), minSliderPos, static_cast<float>(width), sliderPos - minSliderPos));
  }
};
// END CustomLookAndFeel.h
// START DeckGUI.h
/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
 */
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

/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//==============================================================================
/*
 */
class DeckGUI : public Component,
                public Button::Listener,
                public Slider::Listener,
                public FileDragAndDropTarget,
                public Timer
{
public:
  DeckGUI(DJAudioPlayer *player,
          AudioFormatManager &formatManagerToUse,
          AudioThumbnailCache &cacheToUse,
          DJAudioPlayer *otherPlayer);
  ~DeckGUI();

  void paint(Graphics &) override;
  void resized() override;

  /** implement Button::Listener */
  void buttonClicked(Button *) override;

  /** implement Slider::Listener */
  void sliderValueChanged(Slider *slider) override;

  bool isInterestedInFileDrag(const StringArray &files) override;
  void filesDropped(const StringArray &files, int x, int y) override;

  void timerCallback() override;

  void loadWaveform(URL audioURL);

private:
  juce::FileChooser fChooser{"Select a file..."};

  TextButton playButton{"PLAY"};
  TextButton stopButton{"STOP"};
  TextButton loadButton{"LOAD"};
  TextButton autoFadeButton{"AUTO-FADE"};

  Slider volSlider;
  Slider speedSlider;
  Slider posSlider;

  Label volLabel;
  Label speedLabel;
  Label posLabel;

  DJAudioPlayer *player;
  DJAudioPlayer *otherPlayer;

  WaveformDisplay waveformDisplay;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};

// END DeckGUI.h

// START DeckGUI.cpp
/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew
    Modifed: 08 Mar 2025 12:19:32pm
    Updated: MousA

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"

//==============================================================================
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

// END DeckGUI.cpp

// START DJAudioPlayer.h
/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 13 Mar 2020 4:22:22pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public AudioSource
{
public:
  DJAudioPlayer(AudioFormatManager &_formatManager);
  ~DJAudioPlayer();

  void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
  void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
  void releaseResources() override;

  void loadURL(URL audioURL);
  void setGain(double gain);
  void setSpeed(double ratio);
  void setPosition(double posInSecs);
  void setPositionRelative(double pos);

  void start();
  void stop();

  /** get the relative position of the playhead */
  double getPositionRelative();

private:
  AudioFormatManager &formatManager;
  std::unique_ptr<AudioFormatReaderSource> readerSource;
  AudioTransportSource transportSource;
  ResamplingAudioSource resampleSource{&transportSource, false, 2};
};
// END DJAudioPlayer.h

// START DJAudioPlayer.cpp
/*
==============================================================================

DJAudioPlayer.cpp
Created: 13 Mar 2020 4:22:22pm
Author:  matthew

==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(AudioFormatManager &_formatManager)
    : formatManager(_formatManager)
{
}
DJAudioPlayer::~DJAudioPlayer()
{
}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
  transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
  resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}
void DJAudioPlayer::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
  resampleSource.getNextAudioBlock(bufferToFill);
}
void DJAudioPlayer::releaseResources()
{
  transportSource.releaseResources();
  resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(URL audioURL)
{
  auto *reader = formatManager.createReaderFor(audioURL.createInputStream(false));
  if (reader != nullptr) // good file!
  {
    std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader,
                                                                                   true));
    transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
    readerSource.reset(newSource.release());
  }
}
void DJAudioPlayer::setGain(double gain)
{
  if (gain < 0 || gain > 1.0)
  {
    std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
  }
  else
  {
    transportSource.setGain(gain);
  }
}
void DJAudioPlayer::setSpeed(double ratio)
{
  if (ratio < 0 || ratio > 100.0)
  {
    std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
  }
  else
  {
    resampleSource.setResamplingRatio(ratio);
  }
}
void DJAudioPlayer::setPosition(double posInSecs)
{
  transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
  if (pos < 0 || pos > 1.0)
  {
    std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
  }
  else
  {
    double posInSecs = transportSource.getLengthInSeconds() * pos;
    setPosition(posInSecs);
  }
}

void DJAudioPlayer::start()
{
  transportSource.start();
}
void DJAudioPlayer::stop()
{
  transportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{
  return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}
// END DJAudioPlayer.cpp

// START PlaylistComponent.h
/*
  ==============================================================================

    PlaylistComponent.h
    Created: 20 Feb 2025 12:19:32pm
    Author:  MousA

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "vector"
#include "string"
//==============================================================================
/*
 */
/*
  ==============================================================================

    PlaylistComponent.h
    Created: 20 Feb 2025 12:19:32pm
    Author:  MousA

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h" // Include DeckGUI header
#include "WaveformDisplay.h"
#include <vector>
#include <string>
//==============================================================================
// PlaylistComponent class
class PlaylistComponent : public juce::Component,
                          public juce::TableListBoxModel,
                          public Button::Listener

{
public:
  PlaylistComponent(DJAudioPlayer *_player1, DJAudioPlayer *_player2, DeckGUI *_deckGUI1, DeckGUI *_deckGUI2);
  ~PlaylistComponent() override;

  void paint(juce::Graphics &) override;
  void resized() override;
  int getNumRows() override;
  void paintRowBackground(juce::Graphics &,
                          int rowNumber,
                          int width,
                          int height,
                          bool rowIsSelected) override;
  void paintCell(juce::Graphics &,
                 int rowNumber,
                 int columnId,
                 int width,
                 int height,
                 bool rowIsSelected) override;
  Component *refreshComponentForCell(int rowNumber,
                                     int columnId,
                                     bool isRowSelected,
                                     Component *existingComponentToUpdate) override;
  void buttonClicked(Button *button) override;

private:
  DJAudioPlayer *player1;
  DJAudioPlayer *player2;
  DeckGUI *deckGUI1;
  DeckGUI *deckGUI2;
  TextButton loadButton;
  TableListBox tableComponent;
  juce::FileChooser fChooser{"Select a file..."};
  std::vector<std::string> trackTitles;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
// END PlaylistComponent.h

// START PlaylistComponent.cpp
/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 20 Feb 2025 12:19:32pm
    Author:  MousA

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlaylistComponent.h"
#include "iostream"
//==============================================================================
/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 20 Feb 2025 12:19:32pm
    Author:  MousA

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "PlaylistComponent.h"
#include "DJAudioPlayer.h"
#include "iostream"
//==============================================================================
// Constructor
PlaylistComponent::PlaylistComponent(DJAudioPlayer *_player1, DJAudioPlayer *_player2, DeckGUI *_deckGUI1, DeckGUI *_deckGUI2)
    : player1(_player1), player2(_player2), deckGUI1(_deckGUI1), deckGUI2(_deckGUI2)
{
  // Initialize table columns
  tableComponent.getHeader().addColumn("Track Title", 1, 500);
  tableComponent.getHeader().addColumn("Play Deck 1", 2, 150);
  tableComponent.getHeader().addColumn("Play Deck 2", 3, 150);
  tableComponent.setModel(this);
  addAndMakeVisible(tableComponent);

  // Initialize load button
  addAndMakeVisible(loadButton);
  loadButton.setButtonText("Load Tracks");
  loadButton.addListener(this);
}

// Destructor
PlaylistComponent::~PlaylistComponent() {}

// Paint method
void PlaylistComponent::paint(juce::Graphics &g)
{
  g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
  g.setColour(juce::Colours::grey);
  g.drawRect(getLocalBounds(), 1);
  g.setColour(juce::Colours::white);
  g.setFont(juce::FontOptions(14.0f));
  g.drawText("PlaylistComponent", getLocalBounds(), juce::Justification::centred, true);
}

// Resized method
void PlaylistComponent::resized()
{
  tableComponent.setBounds(0, 0, getWidth(), getHeight() - 40); // Adjusted height
  loadButton.setBounds(0, getHeight() - 40, getWidth(), 40);    // Increased height of the load button
}

// Get number of rows
int PlaylistComponent::getNumRows()
{
  return trackTitles.size();
}

// Paint row background
void PlaylistComponent::paintRowBackground(juce::Graphics &g, int rowNumber, int width, int height, bool rowIsSelected)
{
  if (rowIsSelected)
  {
    g.fillAll(juce::Colours::lightblue);
  }
  else
  {
    g.fillAll(juce::Colours::darkgrey);
  }
}

// Paint cell
void PlaylistComponent::paintCell(juce::Graphics &g, int rowNumber, int columnId, int width, int height, bool rowIsSelected)
{
  g.setColour(juce::Colours::black);
  g.setFont(juce::FontOptions(14.0f));
  g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
}

// Refresh component for cell
Component *PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component *existingComponentToUpdate)
{
  if (columnId == 2 || columnId == 3)
  {
    if (existingComponentToUpdate == nullptr)
    {
      TextButton *playButton = new TextButton(columnId == 2 ? "Play Deck 1" : "Play Deck 2");
      String id = String(rowNumber) + (columnId == 2 ? "_1" : "_2");
      playButton->setComponentID(id);
      playButton->addListener(this);
      existingComponentToUpdate = playButton;
      return existingComponentToUpdate;
    }
  }
  return existingComponentToUpdate;
}

// Button clicked
void PlaylistComponent::buttonClicked(Button *button)
{
  if (button == &loadButton)
  {
    auto fileChooserFlags = FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles | FileBrowserComponent::canSelectMultipleItems;
    fChooser.launchAsync(fileChooserFlags, [this](const FileChooser &chooser)
                         {
            auto results = chooser.getResults();
            for (const auto& file : results)
            {
                if (file.existsAsFile())
                {
                    trackTitles.push_back(file.getFullPathName().toStdString());
                }
            }
            tableComponent.updateContent(); });
  }
  else if (button->getButtonText().startsWith("Play Deck"))
  {
    String id = button->getComponentID();
    int rowNumber = id.upToFirstOccurrenceOf("_", false, false).getIntValue();
    bool isDeck1 = id.endsWith("_1");

    std::cout << "Play button clicked " << button->getButtonText() << " Play " << trackTitles[rowNumber] << std::endl;

    if (isDeck1)
    {
      player1->loadURL(URL{File{trackTitles[rowNumber]}});
      player1->start();
      // Load waveform display for Deck 1
      deckGUI1->loadWaveform(URL{File{trackTitles[rowNumber]}});
    }
    else
    {
      player2->loadURL(URL{File{trackTitles[rowNumber]}});
      player2->start();
      // Load waveform display for Deck 2
      deckGUI2->loadWaveform(URL{File{trackTitles[rowNumber]}});
    }
  }
}
// END PlaylistComponent.cpp

// START WaveformDisplay.h
/*
  ==============================================================================

    WaveformDisplay.h
    Created: 14 Mar 2020 3:50:16pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
 */
class WaveformDisplay : public Component,
                        public ChangeListener
{
public:
  WaveformDisplay(AudioFormatManager &formatManagerToUse,
                  AudioThumbnailCache &cacheToUse);
  ~WaveformDisplay();

  void paint(Graphics &) override;
  void resized() override;

  void changeListenerCallback(ChangeBroadcaster *source) override;

  void loadURL(URL audioURL);

  /** set the relative position of the playhead*/
  void setPositionRelative(double pos);

private:
  AudioThumbnail audioThumb;
  bool fileLoaded;
  double position;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};

// END WaveformDisplay.h
// START WaveformDisplay.cpp
/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 14 Mar 2020 3:50:16pm
    Author:  matthew

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager &formatManagerToUse,
                                 AudioThumbnailCache &cacheToUse) : audioThumb(1000, formatManagerToUse, cacheToUse),
                                                                    fileLoaded(false),
                                                                    position(0)

{
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.

  audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint(Graphics &g)
{
  /* This demo code just fills the component's background and
     draws some placeholder text to get you started.

     You should replace everything in this method with your own
     drawing code..
  */

  g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId)); // clear the background

  g.setColour(Colours::grey);
  g.drawRect(getLocalBounds(), 1); // draw an outline around the component

  g.setColour(Colours::orange);
  if (fileLoaded)
  {
    audioThumb.drawChannel(g,
                           getLocalBounds(),
                           0,
                           audioThumb.getTotalLength(),
                           0,
                           1.0f);
    g.setColour(Colours::lightgreen);
    g.drawRect(position * getWidth(), 0, getWidth() / 20, getHeight());
  }
  else
  {
    g.setFont(20.0f);
    g.drawText("File not loaded...", getLocalBounds(),
               Justification::centred, true); // draw some placeholder text
  }
}

void WaveformDisplay::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
}

void WaveformDisplay::loadURL(URL audioURL)
{
  audioThumb.clear();
  fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));
  if (fileLoaded)
  {
    std::cout << "wfd: loaded! " << std::endl;
    repaint();
  }
  else
  {
    std::cout << "wfd: not loaded! " << std::endl;
  }
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster *source)
{
  std::cout << "wfd: change received! " << std::endl;

  repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
  if (pos != position)
  {
    position = pos;
    repaint();
  }
}
// END WaveformDisplay.cpp

// START MainComponent.h
/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent : public AudioAppComponent
{
public:
  //==============================================================================
  MainComponent();
  ~MainComponent();

  //==============================================================================
  void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
  void getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill) override;
  void releaseResources() override;

  //==============================================================================
  void paint(Graphics &g) override;
  void resized() override;

private:
  //==============================================================================
  // Your private member variables go here...

  AudioFormatManager formatManager;
  AudioThumbnailCache thumbCache{100};

  DJAudioPlayer player1{formatManager};
  DeckGUI deckGUI1{&player1, formatManager, thumbCache, &player2};

  DJAudioPlayer player2{formatManager};
  DeckGUI deckGUI2{&player2, formatManager, thumbCache, &player1};

  MixerAudioSource mixerSource;
  PlaylistComponent playlistComponent{&player1, &player2, &deckGUI1, &deckGUI2}; // Pass DeckGUI references

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};

// END MainComponent.h

// START MainComponent.cpp
/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
    : playlistComponent(&player1, &player2, &deckGUI1, &deckGUI2),
      deckGUI1(&player1, formatManager, thumbCache, &player2),
      deckGUI2(&player2, formatManager, thumbCache, &player1)
{
  // Make sure you set the size of the component after
  // you add any child components.
  setSize(800, 800); // Increased height to accommodate the new button and labels

  // Some platforms require permissions to open input channels so request that here
  if (RuntimePermissions::isRequired(RuntimePermissions::recordAudio) && !RuntimePermissions::isGranted(RuntimePermissions::recordAudio))
  {
    RuntimePermissions::request(RuntimePermissions::recordAudio,
                                [&](bool granted)
                                { if (granted)  setAudioChannels (2, 2); });
  }
  else
  {
    // Specify the number of input and output channels that we want to open
    setAudioChannels(0, 2);
  }

  addAndMakeVisible(deckGUI1);
  addAndMakeVisible(deckGUI2);
  addAndMakeVisible(playlistComponent);

  formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
  // This shuts down the audio device and clears the audio source.
  shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
  player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
  player2.prepareToPlay(samplesPerBlockExpected, sampleRate);

  mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

  mixerSource.addInputSource(&player1, false);
  mixerSource.addInputSource(&player2, false);
}
void MainComponent::getNextAudioBlock(const AudioSourceChannelInfo &bufferToFill)
{
  mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
  // This will be called when the audio device stops, or when it is being
  // restarted due to a setting change.

  // For more details, see the help for AudioProcessor::releaseResources()
  player1.releaseResources();
  player2.releaseResources();
  mixerSource.releaseResources();
}

//==============================================================================
void MainComponent::paint(Graphics &g)
{
  // (Our component is opaque, so we must completely fill the background with a solid colour)
  g.fillAll(getLookAndFeel().findColour(ResizableWindow::backgroundColourId));

  // You can add your drawing code here!
}

void MainComponent::resized()
{
  deckGUI1.setBounds(0, 0, getWidth() / 2, getHeight() / 2);
  deckGUI2.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight() / 2);
  playlistComponent.setBounds(0, getHeight() / 2, getWidth(), getHeight() / 2);
}

// END MainComponent.cpp

// START Main.cpp
/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "MainComponent.h"

//==============================================================================
class OtoDecksApplication : public JUCEApplication
{
public:
  //==============================================================================
  OtoDecksApplication() {}
  const String getApplicationName() override { return ProjectInfo::projectName; }
  const String getApplicationVersion() override { return ProjectInfo::versionString; }
  bool moreThanOneInstanceAllowed() override { return true; }

  //==============================================================================
  void initialise(const String &commandLine) override
  {
    // This method is where you should put your application's initialisation code..

    mainWindow.reset(new MainWindow(getApplicationName()));
  }

  void shutdown() override
  {
    // Add your application's shutdown code here..

    mainWindow = nullptr; // (deletes our window)
  }

  //==============================================================================
  void systemRequestedQuit() override
  {
    // This is called when the app is being asked to quit: you can ignore this
    // request and let the app carry on running, or call quit() to allow the app to close.
    quit();
  }

  void anotherInstanceStarted(const String &commandLine) override
  {
    // When another instance of the app is launched while this one is running,
    // this method is invoked, and the commandLine parameter tells you what
    // the other instance's command-line arguments were.
  }

  //==============================================================================
  /*
      This class implements the desktop window that contains an instance of
      our MainComponent class.
  */
  class MainWindow : public DocumentWindow
  {
  public:
    MainWindow(String name) : DocumentWindow(name,
                                             Desktop::getInstance().getDefaultLookAndFeel().findColour(ResizableWindow::backgroundColourId),
                                             DocumentWindow::allButtons)
    {
      setUsingNativeTitleBar(true);
      setContentOwned(new MainComponent(), true);

#if JUCE_IOS || JUCE_ANDROID
      setFullScreen(true);
#else
      setResizable(true, true);
      centreWithSize(getWidth(), getHeight());
#endif

      setVisible(true);
    }

    void closeButtonPressed() override
    {
      // This is called when the user tries to close this window. Here, we'll just
      // ask the app to quit when this happens, but you can change this to do
      // whatever you need.
      JUCEApplication::getInstance()->systemRequestedQuit();
    }

    /* Note: Be careful if you override any DocumentWindow methods - the base
       class uses a lot of them, so by overriding you might break its functionality.
       It's best to do all your work in your content component instead, but if
       you really have to override any DocumentWindow methods, make sure your
       subclass also calls the superclass's method.
    */

  private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
  };

private:
  std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(OtoDecksApplication)

// END Main.cpp