#pragma once

#include "Animation.hpp"
#include "KeyframeProperty.hpp"
#include "Transition.hpp"

#include <span>

namespace mc {

template<typename T>
struct TransitionProperty : KeyframeProperty<T>
{
    explicit TransitionProperty(Transition& transition) : TransitionProperty{transition, {}, {}} {}

    explicit TransitionProperty(Animation& animation) : TransitionProperty{animation, {}, {}} {}

    TransitionProperty(Transition& transition, T const& from, T const& to)
        : KeyframeProperty<T>{from, to}
        , _position{[t = &transition] { return t->position(); }}
    {
        _frames.emplace_back(from, 0.0);
        _frames.emplace_back(to, 1.0);
    }

    TransitionProperty(Animation& animation, T const& from, T const& to)
        : KeyframeProperty<T>{from, to}
        , _position{[a = &animation] { return a->position(); }}
    {
        _frames.emplace_back(from, 0.0);
        _frames.emplace_back(to, 1.0);
    }

    [[nodiscard]] auto get() const -> T
    {
        jassert(_position);
        return KeyframeProperty<T>::get(_position());
    }

private:
    std::function<double()> _position;
    std::vector<Keyframe<T>> _frames{};
};

}  // namespace mc
