#pragma once

#include "neo/animations/KeyframeInterpolator.hpp"

#include <span>

namespace neo {

template<typename T>
struct Keyframe
{
    T value{};
    double position{};
};

template<typename T>
Keyframe(T) -> Keyframe<T>;

template<typename T>
Keyframe(T, double) -> Keyframe<T>;

template<typename T>
struct KeyframeProperty
{
    KeyframeProperty() = default;
    KeyframeProperty(T const& from, T const& to);

    auto setKeyframes(T const& from, T const& to) -> void;
    auto setKeyframes(std::span<Keyframe<T> const> frames) -> void;

    [[nodiscard]] auto get(double t) const -> T;

private:
    std::vector<Keyframe<T>> _frames{};
};

template<typename T>
KeyframeProperty<T>::KeyframeProperty(T const& from, T const& to)
{
    setKeyframes(from, to);
}

template<typename T>
auto KeyframeProperty<T>::setKeyframes(T const& from, T const& to) -> void
{
    _frames.clear();
    _frames.push_back(Keyframe{from, 0.0});
    _frames.push_back(Keyframe{to, 1.0});
}

template<typename T>
auto KeyframeProperty<T>::setKeyframes(std::span<Keyframe<T> const> frames) -> void
{
    _frames.resize(frames.size());
    std::copy(frames.begin(), frames.end(), _frames.begin());
}

template<typename T>
auto KeyframeProperty<T>::get(double t) const -> T
{
    jassert(_frames.size() >= 2);

    if (t <= 0.0) {
        return _frames.front().value;
    }
    if (t >= 1.0) {
        return _frames.back().value;
    }

    auto const cmp  = [](auto v, auto const& k) { return v < k.position; };
    auto const to   = std::upper_bound(begin(_frames), end(_frames), t, cmp);
    auto const from = std::prev(to);

    jassert(to != _frames.begin());
    jassert(to != _frames.end());

    auto const pos = (t - from->position) / (to->position - from->position);
    return KeyframeInterpolator<T>{}(from->value, to->value, pos);
}

}  // namespace neo
