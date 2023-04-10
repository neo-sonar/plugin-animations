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
    Transition<juce::Rectangle<float>> _margin{{.parent = this}, {}, {}};
    Transition<float> _corner{{.parent = this}, {}, {}};
    Transition<float> _font{{.parent = this}, {}, {}};
    Transition<juce::Colour> _background{{.parent = this}, {}, {}};
    Transition<juce::Colour> _text{{.parent = this}, {}, {}};

    MouseHoverListener _hoverListener{this};
};
}  // namespace mc
