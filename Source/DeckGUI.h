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
