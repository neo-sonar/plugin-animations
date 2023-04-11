#pragma once

#include "Animation.hpp"
#include "Transition.hpp"
#include "TransitionTraits.hpp"

#include <span>

namespace mc {

template<typename T>
struct Keyframe
{
    T value{};
    double position{};
};

template<typename T>
struct TransitionProperty
{
    explicit TransitionProperty(Transition& transition) : TransitionProperty{transition, {}, {}} {}

    explicit TransitionProperty(Animation& animation) : TransitionProperty{animation, {}, {}} {}

    TransitionProperty(Transition& transition, T const& from, T const& to)
        : _position{[t = &transition] { return t->position(); }}
    {
        _frames.emplace_back(from, 0.0);
        _frames.emplace_back(to, 1.0);
    }

    TransitionProperty(Animation& animation, T const& from, T const& to)
        : _position{[a = &animation] { return a->position(); }}
    {
        _frames.emplace_back(from, 0.0);
        _frames.emplace_back(to, 1.0);
    }

    auto keyframes(T const& from, T const& to)
    {
        _frames.clear();
        _frames.emplace_back(from, 0.0);
        _frames.emplace_back(to, 1.0);
    }

    auto keyframes(std::span<Keyframe<T> const> frames)
    {
        _frames.resize(frames.size());
        std::copy(frames.begin(), frames.end(), _frames.begin());
    }

    [[nodiscard]] auto get() const -> T
    {
        jassert(_position);
        jassert(_frames.size() >= 2);

        auto const t = _position();
        if (t <= 0.0) { return _frames.front().value; }
        if (t >= 1.0) { return _frames.back().value; }

        auto const cmp  = [](auto value, auto const& k) { return value < k.position; };
        auto const to   = std::upper_bound(begin(_frames), end(_frames), t, cmp);
        auto const from = std::prev(to);

        jassert(to != _frames.begin());
        jassert(to != _frames.end());

        auto const pos = (t - from->position) / (to->position - from->position);
        return TransitionTraits<T>::interpolate(from->value, to->value, pos);
    }

private:
    std::function<double()> _position;
    std::vector<Keyframe<T>> _frames{};
};

}  // namespace mc
