#pragma once

#include "KeyframeTraits.hpp"

namespace mc {

template<typename T>
struct Keyframe
{
    T value{};
    double position{};
};

}  // namespace mc
