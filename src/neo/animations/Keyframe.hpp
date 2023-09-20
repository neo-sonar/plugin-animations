#pragma once

#include "neo/animations/KeyframeTraits.hpp"

namespace mc {

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

}  // namespace mc
