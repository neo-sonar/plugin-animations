#pragma once

#include "TransitionTraits.hpp"

namespace mc {

template<typename T>
struct TransitionProperty
{
    TransitionProperty(T const& a, T const& b) : _keyframes{a, b} {}

    auto keyframes(T const& a, T const& b) { _keyframes = {a, b}; }

    [[nodiscard]] auto get(double t) const -> T
    {
        return TransitionTraits<T>::interpolate(_keyframes.front(), _keyframes.back(), t);
    }

private:
    std::array<T, 2> _keyframes;
};

}  // namespace mc
