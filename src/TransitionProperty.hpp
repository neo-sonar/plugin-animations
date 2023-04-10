#pragma once

#include "TransitionTraits.hpp"

namespace mc {

template<typename T>
struct TransitionProperty
{
    constexpr TransitionProperty() = default;

    constexpr TransitionProperty(T const& a, T const& b) : _keyframes{a, b} {}

    constexpr auto keyframes(T const& a, T const& b) { _keyframes = {a, b}; }

    [[nodiscard]] constexpr auto get(double t) const -> T
    {
        return TransitionTraits<T>::interpolate(_keyframes.front(), _keyframes.back(), t);
    }

private:
    std::array<T, 2> _keyframes{};
};

}  // namespace mc
