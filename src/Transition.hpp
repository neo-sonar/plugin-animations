#pragma once

#include "AnimationTimer.hpp"
#include "TimingFunction.hpp"

#include <juce_gui_basics/juce_gui_basics.h>

namespace mc {

struct TransitionSpec
{
    std::chrono::milliseconds duration{600};
    std::chrono::milliseconds delay{0};
    std::function<double(double)> timingFunction{TimingFunction::Linear};
};

struct Transition
{
    explicit Transition(juce::Component* parent, TransitionSpec const& spec = {})
        : _parent{parent}
        , _spec{spec}
    {
        jassert(_parent != nullptr);

        duration(_spec.duration);
        delay(_spec.delay);

        onTick        = [this] { _parent->repaint(); };
        _timer.onTick = [this] {
            if (this->onTick) { this->onTick(); }
        };
    }

    [[nodiscard]] auto position() const -> double { return _spec.timingFunction(_timer.position()); }

    auto forward() -> void { _timer.play(AnimationDirection::normal); }

    auto backward() -> void { _timer.play(AnimationDirection::reverse); }

    auto duration(std::chrono::milliseconds ms) -> void
    {
        _spec.duration = ms;
        _timer.duration(ms);
    }

    auto delay(std::chrono::milliseconds ms) -> void
    {
        _spec.delay = ms;
        _timer.delay(ms);
    }

    std::function<void()> onTick;

private:
    juce::Component* _parent{nullptr};
    TransitionSpec _spec;
    AnimationTimer _timer{_parent, false};
};

}  // namespace mc
