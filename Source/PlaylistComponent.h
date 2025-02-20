/*
  ==============================================================================

    PlaylistComponent.h
    Created: 20 Feb 2025 12:19:32pm
    Author:  MousA

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
 */
class PlaylistComponent : public juce::Component
{
public:
  PlaylistComponent();
  ~PlaylistComponent() override;

  void paint(juce::Graphics &) override;
  void resized() override;

private:
  TableListBox tableComponent;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};
