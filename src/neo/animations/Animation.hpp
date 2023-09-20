#pragma once

#include "neo/animations/AnimationTimer.hpp"
#include "neo/animations/TimingFunction.hpp"

#include <juce_gui_basics/juce_gui_basics.h>

namespace neo {

struct AnimationSpec
{
    std::chrono::milliseconds duration{600};
    std::chrono::milliseconds delay{0};
    bool isLooping{false};
    std::function<double(double)> timingFunction{TimingFunction::Linear};
};

struct Animation
{
    explicit Animation(juce::Component* parent, AnimationSpec const& spec = {}) : _parent{parent}, _spec{spec}
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

    auto play(AnimationDirection direction = AnimationDirection::normal) -> void { _timer.play(direction); }

    [[nodiscard]] auto getPosition() const -> double { return _spec.timingFunction(_timer.getPosition()); }

    std::function<void()> onTick;

private:
    juce::Component* _parent{nullptr};
    AnimationSpec _spec;
    AnimationTimer _timer{_parent, _spec.isLooping};
};

}  // namespace neo
