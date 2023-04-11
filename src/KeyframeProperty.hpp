#pragma once

#include "Keyframe.hpp"

#include <span>

namespace mc {

template<typename T>
struct KeyframeProperty
{
    KeyframeProperty() = default;

    KeyframeProperty(T const& from, T const& to) { keyframes(from, to); }

    auto keyframes(T const& from, T const& to) -> void
    {
        _frames.clear();
        _frames.emplace_back(from, 0.0);
        _frames.emplace_back(to, 1.0);
    }

    auto keyframes(std::span<Keyframe<T> const> frames) -> void
    {
        _frames.resize(frames.size());
        std::copy(frames.begin(), frames.end(), _frames.begin());
    }

    [[nodiscard]] auto get(double t) const -> T
    {
        jassert(_frames.size() >= 2);

        if (t <= 0.0) { return _frames.front().value; }
        if (t >= 1.0) { return _frames.back().value; }

        auto const cmp  = [](auto v, auto const& k) { return v < k.position; };
        auto const to   = std::upper_bound(begin(_frames), end(_frames), t, cmp);
        auto const from = std::prev(to);

        jassert(to != _frames.begin());
        jassert(to != _frames.end());

        auto const pos = (t - from->position) / (to->position - from->position);
        return KeyframeTraits<T>::interpolate(from->value, to->value, pos);
    }

private:
    std::vector<Keyframe<T>> _frames{};
};

}  // namespace mc
