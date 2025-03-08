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
