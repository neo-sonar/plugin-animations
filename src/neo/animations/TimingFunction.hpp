#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace neo {

struct CubicInterpolation
{
    template<typename T>
    [[nodiscard]] auto operator()(juce::Point<T> p1, juce::Point<T> p2, T t) const noexcept -> juce::Point<T>
    {

        auto const x0 = T{0};
        auto const y0 = T{0};
        auto const x1 = p1.x;
        auto const y1 = p1.y;
        auto const x2 = p2.x;
        auto const y2 = p2.y;
        auto const x3 = T{1};
        auto const y3 = T{1};

        auto const y = [=](auto ts) {
            return static_cast<T>(
                std::pow(1 - ts, 3) * y0 + 3 * std::pow(1 - ts, 2) * ts * y1 + 3 * (1 - ts) * std::pow(ts, 2) * y2
                + std::pow(ts, 3) * y3
            );
        };

        auto const x = [=](auto ts) {
            return static_cast<T>(
                std::pow(1 - ts, 3) * x0 + 3 * std::pow(1 - ts, 2) * ts * x1 + 3 * (1 - ts) * std::pow(ts, 2) * x2
                + std::pow(ts, 3) * x3
            );
        };

        return {x(t), y(t)};
    }
};

inline constexpr auto makeTransition = [](int index, float ts) {
    if (index == 1) {
        return CubicInterpolation{}({0.0F, 0.0F}, {1.0F, 1.0F}, ts);
    }
    if (index == 2) {
        return CubicInterpolation{}({0.0F, 0.0F}, {0.58F, 1.0F}, ts);
    }
    if (index == 3) {
        return CubicInterpolation{}({0.42F, 0.0F}, {0.58F, 1.0F}, ts);
    }
    if (index == 4) {
        return CubicInterpolation{}({0.68F, -0.2F}, {0.32F, 1.2F}, ts);
    }
    return juce::Point<float>{};
};

struct TimingFunction
{
    static constexpr auto Linear        = [](double t) { return CubicInterpolation{}({0.0F, 0.0}, {1.0, 1.0}, t).y; };
    static constexpr auto EaseOut       = [](double t) { return CubicInterpolation{}({0.0, 0.0}, {0.58, 1.0}, t).y; };
    static constexpr auto EaseInOut     = [](double t) { return CubicInterpolation{}({0.42, 0.0}, {0.58, 1.0}, t).y; };
    static constexpr auto EaseInOutBack = [](double t) { return CubicInterpolation{}({0.68, -0.2}, {0.32, 1.2}, t).y; };
};
}  // namespace neo
