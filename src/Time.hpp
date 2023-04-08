#pragma once

#include <chrono>

namespace mc {

template<typename T>
using Microseconds = std::chrono::duration<T, std::micro>;

template<typename T>
using Milliseconds = std::chrono::duration<T, std::milli>;

template<typename T>
using Seconds = std::chrono::duration<T>;

using SystemClock = std::chrono::system_clock;

}  // namespace mc
