#pragma once

#include "MouseHoverListener.hpp"
#include "Transition.hpp"

#include <juce_gui_extra/juce_gui_extra.h>

namespace mc {
struct AnimatedButton final : juce::Button
{
    explicit AnimatedButton(juce::String const& name);
    ~AnimatedButton() override = default;

    auto paintButton(juce::Graphics& g, bool isHighlighted, bool isDown) -> void override;
    auto resized() -> void override;

private:
    Transition<juce::Rectangle<float>> _margin{this};
    Transition<float> _corner{this};
    Transition<float> _font{this};
    Transition<juce::Colour> _background{this};
    Transition<juce::Colour> _text{this};

    MouseHoverListener _hoverListener{this};
};
}  // namespace mc
