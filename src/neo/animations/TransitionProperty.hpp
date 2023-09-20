#pragma once

#include "neo/animations/Animation.hpp"
#include "neo/animations/KeyframeProperty.hpp"
#include "neo/animations/Transition.hpp"

#include <span>

namespace neo {

template<typename T>
struct TransitionProperty : KeyframeProperty<T>
{
    explicit TransitionProperty(Transition& transition) : TransitionProperty{transition, {}, {}} {}

    TransitionProperty(Transition& transition, T const& from, T const& to)
        : KeyframeProperty<T>{from, to}
        , _transition{transition}
    {}

    [[nodiscard]] auto get() const -> T { return KeyframeProperty<T>::get(_transition.getPosition()); }

private:
    Transition& _transition;
};

template<typename T>
struct AnimationProperty : KeyframeProperty<T>
{
    explicit AnimationProperty(Animation& animation) : AnimationProperty{animation, {}, {}} {}

    AnimationProperty(Animation& animation, T const& from, T const& to)
        : KeyframeProperty<T>{from, to}
        , _animation{animation}
    {}

    [[nodiscard]] auto get() const -> T { return KeyframeProperty<T>::get(_animation.getPosition()); }

private:
    Animation& _animation;
};

}  // namespace neo
