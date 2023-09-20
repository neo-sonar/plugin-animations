#pragma once

#include "neo/animations/KeyframeTimer.hpp"
#include "neo/animations/TimingFunction.hpp"

#include <juce_gui_basics/juce_gui_basics.h>

namespace neo {

struct Animation
{
    struct Spec
    {
        std::chrono::milliseconds duration{600};
        std::chrono::milliseconds delay{0};
        bool isLooping{false};
        std::function<double(double)> timingFunction{TimingFunction::Linear};
    };

    explicit Animation(juce::Component* parent, Spec spec);

    auto play(KeyframeTimer::Direction direction = KeyframeTimer::Direction::normal) -> void;

    auto setDuration(std::chrono::milliseconds ms) -> void;
    auto setDelay(std::chrono::milliseconds ms) -> void;

    [[nodiscard]] auto getPosition() const -> double;

    std::function<void()> onTick;

private:
    juce::Component* _parent{nullptr};
    Spec _spec;
    KeyframeTimer _timer{_parent, _spec.isLooping};
};

}  // namespace neo
