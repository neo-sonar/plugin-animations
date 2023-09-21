#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace neo {

template<typename T>
[[nodiscard]] auto cubicBezier(juce::Point<T> p1, juce::Point<T> p2, T t) noexcept -> juce::Point<T>
{
    auto const x = [=](auto ts) {
        auto const x0 = T{0};
        auto const x1 = p1.x;
        auto const x2 = p2.x;
        auto const x3 = T{1};

        return static_cast<T>(
            std::pow(1 - ts, 3) * x0               //
            + 3 * std::pow(1 - ts, 2) * ts * x1    //
            + 3 * (1 - ts) * std::pow(ts, 2) * x2  //
            + std::pow(ts, 3) * x3                 //
        );
    };

    auto const y = [=](auto ts) {
        auto const y0 = T{0};
        auto const y1 = p1.y;
        auto const y2 = p2.y;
        auto const y3 = T{1};

        return static_cast<T>(
            std::pow(1 - ts, 3) * y0               //
            + 3 * std::pow(1 - ts, 2) * ts * y1    //
            + 3 * (1 - ts) * std::pow(ts, 2) * y2  //
            + std::pow(ts, 3) * y3                 //
        );
    };

    return {x(t), y(t)};
}

struct TimingFunction
{
    static constexpr auto Linear        = [](double t) { return cubicBezier({0.0F, 0.0}, {1.0, 1.0}, t).y; };
    static constexpr auto EaseOut       = [](double t) { return cubicBezier({0.0, 0.0}, {0.58, 1.0}, t).y; };
    static constexpr auto EaseInOut     = [](double t) { return cubicBezier({0.42, 0.0}, {0.58, 1.0}, t).y; };
    static constexpr auto EaseInOutBack = [](double t) { return cubicBezier({0.68, -0.2}, {0.32, 1.2}, t).y; };
};
}  // namespace neo
