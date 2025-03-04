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
class DeckGUI : public Component,
                public Button::Listener,
                public Slider::Listener,
                public FileDragAndDropTarget,
                public Timer
{
public:
  DeckGUI(DJAudioPlayer *player,
          AudioFormatManager &formatManagerToUse,
          AudioThumbnailCache &cacheToUse);
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

private:
  juce::FileChooser fChooser{"Select a file..."};

  TextButton playButton{"PLAY"};
  TextButton stopButton{"STOP"};
  TextButton loadButton{"LOAD"};

  Slider volSlider;
  Slider speedSlider;
  Slider posSlider;

  WaveformDisplay waveformDisplay;

  DJAudioPlayer *player;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};
// END DeckGUI.h
// START DeckGUI.cpp
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
                 AudioThumbnailCache &cacheToUse) : player(_player),
                                                    waveformDisplay(formatManagerToUse, cacheToUse)
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

  startTimer(500);
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
  double rowH = getHeight() / 8;
  playButton.setBounds(0, 0, getWidth(), rowH);
  stopButton.setBounds(0, rowH, getWidth(), rowH);
  volSlider.setBounds(0, rowH * 2, getWidth(), rowH);
  speedSlider.setBounds(0, rowH * 3, getWidth(), rowH);
  posSlider.setBounds(0, rowH * 4, getWidth(), rowH);
  waveformDisplay.setBounds(0, rowH * 5, getWidth(), rowH * 2);
  loadButton.setBounds(0, rowH * 7, getWidth(), rowH);
}

void DeckGUI::buttonClicked(Button *button)
{
  if (button == &playButton)
  {
    std::cout << "Play button was clicked " << std::endl;
    player->start();
  }
  if (button == &stopButton)
  {
    std::cout << "Stop button was clicked " << std::endl;
    player->stop();
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
            } });
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
class PlaylistComponent : public juce::Component,
                          public juce::TableListBoxModel,
                          public Button::Listener

{
public:
  PlaylistComponent();
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
  TableListBox tableComponent;
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
PlaylistComponent::PlaylistComponent()
{
  // In your constructor, you should add any child components, and
  // initialise any special settings that your component needs.

  trackTitles.push_back("Track 1");
  trackTitles.push_back("Track 2");
  trackTitles.push_back("Track 3");
  tableComponent.getHeader().addColumn("Track Title", 1, 400);
  tableComponent.getHeader().addColumn("Track Title", 2, 400);
  tableComponent.getHeader().addColumn("Track Title", 3, 400);
  tableComponent.setModel(this);
  addAndMakeVisible(tableComponent);
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint(juce::Graphics &g)
{
  /* This demo code just fills the component's background and
     draws some placeholder text to get you started.

     You should replace everything in this method with your own
     drawing code..
  */

  g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId)); // clear the background

  g.setColour(juce::Colours::grey);
  g.drawRect(getLocalBounds(), 1); // draw an outline around the component

  g.setColour(juce::Colours::white);
  g.setFont(juce::FontOptions(14.0f));
  g.drawText("PlaylistComponent", getLocalBounds(),
             juce::Justification::centred, true); // draw some placeholder text
}

void PlaylistComponent::resized()
{
  // This method is where you should set the bounds of any child
  // components that your component contains..
  tableComponent.setBounds(0, 0, getWidth(), getHeight());
}

int PlaylistComponent::getNumRows()
{
  return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(juce::Graphics &g,
                                           int rowNumber,
                                           int width,
                                           int height,
                                           bool rowIsSelected)
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

void PlaylistComponent::paintCell(juce::Graphics &g,
                                  int rowNumber,
                                  int columnId,
                                  int width,
                                  int height,
                                  bool rowIsSelected)
{
  g.setColour(juce::Colours::black);
  g.setFont(juce::FontOptions(14.0f));
  g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, juce::Justification::centredLeft, true);
}

Component *PlaylistComponent::refreshComponentForCell(int rowNumber,
                                                      int columnId,
                                                      bool isRowSelected,
                                                      Component *existingComponentToUpdate)
{
  if (columnId == 2)
  {
    if (existingComponentToUpdate == nullptr)
    {
      TextButton *playButton = new TextButton("Play");
      String id = String(rowNumber);
      playButton->setComponentID(id);
      playButton->addListener(this);
      existingComponentToUpdate = playButton;
      return existingComponentToUpdate;
    }
  }
  return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button *button)
{
  if (button->getButtonText() == "Play")
  {
    int id = button->getComponentID().getIntValue();

    std::cout << "Play button clicked " << button->getComponentID() << " Play " << trackTitles[id] << std::endl;
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
  DeckGUI deckGUI1{&player1, formatManager, thumbCache};

  DJAudioPlayer player2{formatManager};
  DeckGUI deckGUI2{&player2, formatManager, thumbCache};

  MixerAudioSource mixerSource;
  PlaylistComponent playlistComponent;

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
{
  // Make sure you set the size of the component after
  // you add any child components.
  setSize(800, 600);

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