#pragma once

#include "AnimatedProperty.hpp"
#include <juce_gui_extra/juce_gui_extra.h>

namespace mc {
struct AnimatedButton final : juce::Button
{
    explicit AnimatedButton(juce::String const& name);
    ~AnimatedButton() override = default;

    auto paintButton(juce::Graphics& g, bool isHighlighted, bool isDown) -> void override;

private:
    AnimatedProperty<float> _margin{this, 0.0F, 0.125F};
    AnimatedProperty<float> _corner{this, 0.0F, 8.0F};
    AnimatedProperty<float> _font{this, 1.0F, 2.0F};
    AnimatedColor _background{this, juce::Colours::white, juce::Colours::green};
    AnimatedColor _text{this, juce::Colours::black, juce::Colours::white};
};
}  // namespace mc
