#pragma once

#include <juce_graphics/juce_graphics.h>

#include <concepts>

namespace neo {

template<typename T>
struct KeyframeInterpolator;

template<std::integral T>
struct KeyframeInterpolator<T>
{
    [[nodiscard]] constexpr auto operator()(T a, T b, double t) const -> T
    {
        return static_cast<T>(std::lerp(a, b, t));
    }
};

template<std::floating_point T>
struct KeyframeInterpolator<T>
{
    [[nodiscard]] constexpr auto operator()(T a, T b, double t) const -> T
    {
        return std::lerp(a, b, static_cast<T>(t));
    }
};

template<>
struct KeyframeInterpolator<juce::Colour>
{
    [[nodiscard]] auto operator()(juce::Colour a, juce::Colour b, double t) const -> juce::Colour
    {
        return a.interpolatedWith(b, static_cast<float>(t));
    }
};

template<typename T>
struct KeyframeInterpolator<juce::Rectangle<T>>
{
    [[nodiscard]] auto operator()(juce::Rectangle<T> const& a, juce::Rectangle<T> const& b, double t) const
        -> juce::Rectangle<T>
    {
        if constexpr (std::is_floating_point_v<T>) {
            return {
                std::lerp(a.getX(), b.getX(), static_cast<T>(t)),
                std::lerp(a.getY(), b.getY(), static_cast<T>(t)),
                std::lerp(a.getWidth(), b.getWidth(), static_cast<T>(t)),
                std::lerp(a.getHeight(), b.getHeight(), static_cast<T>(t)),
            };
        } else {
            using Double = KeyframeInterpolator<juce::Rectangle<double>>;
            return Double{}(a.toDouble(), b.toDouble(), t).template toType<T>();
        }
    }
};

}  // namespace neo
