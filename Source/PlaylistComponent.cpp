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
PlaylistComponent::PlaylistComponent(DJAudioPlayer *_player1, DJAudioPlayer *_player2)
    : player1(_player1), player2(_player2)
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
  tableComponent.setBounds(0, 0, getWidth(), getHeight() - 30);
  loadButton.setBounds(0, getHeight() - 30, getWidth(), 30);
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
    auto fileChooserFlags = FileBrowserComponent::openMode | FileBrowserComponent::canSelectFiles;
    fChooser.launchAsync(fileChooserFlags, [this](const FileChooser &chooser)
                         {
      File file = chooser.getResult();
      if (file.existsAsFile())
      {
        trackTitles.push_back(file.getFullPathName().toStdString());
        tableComponent.updateContent();
      } });
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
    }
    else
    {
      player2->loadURL(URL{File{trackTitles[rowNumber]}});
      player2->start();
    }
  }
}