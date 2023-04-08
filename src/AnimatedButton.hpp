#pragma once

#include "AnimatedProperty.hpp"
#include <juce_gui_extra/juce_gui_extra.h>

namespace mc {
struct AnimatedButton final : juce::Button
{
    explicit AnimatedButton(juce::String const& name);
    ~AnimatedButton() override = default;

    auto paintButton(juce::Graphics& g, bool isHighlighted, bool isDown) -> void override;
    auto resized() -> void override;

private:
    AnimatedProperty<juce::Rectangle<float>> _margin{{.parent = this}, {}, {}};
    AnimatedProperty<float> _corner{{.parent = this}, {}, {}};
    AnimatedProperty<float> _font{{.parent = this}, {}, {}};
    AnimatedColor _background{{.parent = this}, {}, {}};
    AnimatedColor _text{{.parent = this}, {}, {}};
};
}  // namespace mc
