#pragma once

#include "neo/animations/AnimationTimer.hpp"
#include "neo/animations/TimingFunction.hpp"

#include <juce_gui_basics/juce_gui_basics.h>

namespace mc {

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

        duration(_spec.duration);
        delay(_spec.delay);

        onTick        = [this] { _parent->repaint(); };
        _timer.onTick = [this] {
            if (this->onTick) {
                this->onTick();
            }
        };
    }

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

    auto play(AnimationDirection direction = AnimationDirection::normal) -> void { _timer.play(direction); }

    [[nodiscard]] auto position() const -> double { return _spec.timingFunction(_timer.position()); }

    std::function<void()> onTick;

private:
    juce::Component* _parent{nullptr};
    AnimationSpec _spec;
    // AnimationTimer _timer{_parent, _spec.isLooping};
    v2::Timer _timer{_parent, _spec.isLooping};
};

}  // namespace mc
