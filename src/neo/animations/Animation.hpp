#pragma once

#include "neo/animations/KeyframeProperty.hpp"
#include "neo/animations/KeyframeTimer.hpp"
#include "neo/animations/TimingFunction.hpp"

#include <juce_gui_basics/juce_gui_basics.h>

#include <span>

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

    Animation(juce::Component* parent, Spec spec);

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

template<typename T, typename Interpolator = KeyframeInterpolator<T>>
struct AnimationProperty : KeyframeProperty<T, Interpolator>
{
    explicit AnimationProperty(Animation& animation) : AnimationProperty{animation, {}, {}} {}

    AnimationProperty(Animation& animation, T const& from, T const& to)
        : KeyframeProperty<T, Interpolator>{from, to}
        , _animation{animation}
    {}

    [[nodiscard]] auto get() const -> T { return KeyframeProperty<T, Interpolator>::get(_animation.getPosition()); }

private:
    Animation& _animation;
};

}  // namespace neo
