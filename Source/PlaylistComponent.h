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
