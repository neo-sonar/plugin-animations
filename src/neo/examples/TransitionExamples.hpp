#pragma once

#include "neo/animations/Transition.hpp"

#include <juce_gui_basics/juce_gui_basics.h>

namespace neo {

struct TransitionExamples final : juce::Component
{
    TransitionExamples();
    ~TransitionExamples() override = default;

    auto paint(juce::Graphics& g) -> void override;
    auto resized() -> void override;

private:
    [[nodiscard]] static auto makeTransition() -> Transition::Spec
    {
        return {
            .duration = std::chrono::milliseconds{2000},
        };
    }

    Transition _tran{this, makeTransition()};
    TransitionProperty<float> _trim{_tran};

    juce::ComboBox _transition{"Transition"};
    juce::Slider _duration{juce::Slider::LinearHorizontal, juce::Slider::TextBoxRight};
    juce::Rectangle<float> _canvas{};
};

}  // namespace neo
