#pragma once

#include "neo/animations/Transition.hpp"
#include "neo/components/MouseHoverListener.hpp"

#include <juce_gui_extra/juce_gui_extra.h>

namespace neo {
struct AnimatedButton final : juce::Button
{
    explicit AnimatedButton(juce::String const& name);
    ~AnimatedButton() override = default;

    auto paintButton(juce::Graphics& g, bool isHighlighted, bool isDown) -> void override;
    auto resized() -> void override;

private:
    Transition _transition{this, Transition::Spec{}};
    TransitionProperty<float> _font{_transition};
    TransitionProperty<float> _corner{_transition};
    TransitionProperty<juce::Rectangle<float>> _layout{_transition};
    TransitionProperty<juce::Colour> _backgroundColor{_transition};
    TransitionProperty<juce::Colour> _textColor{_transition};

    MouseHoverListener _hoverListener{this};
};
}  // namespace neo
