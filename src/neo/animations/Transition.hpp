#pragma once

#include "neo/animations/AnimationTimer.hpp"
#include "neo/animations/TimingFunction.hpp"

#include <juce_gui_basics/juce_gui_basics.h>

namespace neo {

struct TransitionSpec
{
    std::chrono::milliseconds duration{600};
    std::chrono::milliseconds delay{0};
    std::function<double(double)> timingFunction{TimingFunction::Linear};
};

struct Transition
{
    explicit Transition(juce::Component* parent, TransitionSpec const& spec = {}) : _parent{parent}, _spec{spec}
    {
        jassert(_parent != nullptr);

        setDuration(_spec.duration);
        setDelay(_spec.delay);

        onTick        = [this] { _parent->repaint(); };
        _timer.onTick = [this] {
            if (this->onTick) {
                this->onTick();
            }
        };
    }

    auto forward() -> void { _timer.play(AnimationDirection::normal); }

    auto backward() -> void { _timer.play(AnimationDirection::reverse); }

    [[nodiscard]] auto getPosition() const -> double { return _spec.timingFunction(_timer.getPosition()); }

    auto setDuration(std::chrono::milliseconds ms) -> void
    {
        _spec.duration = ms;
        _timer.setDuration(ms);
    }

    auto setDelay(std::chrono::milliseconds ms) -> void
    {
        _spec.delay = ms;
        _timer.setDelay(ms);
    }

    std::function<void()> onTick;

private:
    juce::Component* _parent{nullptr};
    TransitionSpec _spec;
    AnimationTimer _timer{_parent, false};
};

}  // namespace neo
