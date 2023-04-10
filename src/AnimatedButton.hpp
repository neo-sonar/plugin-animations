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
    enum Index
    {
        FontIndex = 0,

        MarginIndex = 0,
        CornerIndex = 1,

        BackgroundIndex = 0,
        TextIndex       = 1,
    };

    Transition<float> _font{this};
    Transition<juce::Rectangle<float>, float> _layout{this};
    Transition<juce::Colour, juce::Colour> _color{this};

    MouseHoverListener _hoverListener{this};
};
}  // namespace mc
