#pragma once

#include <juce_graphics/juce_graphics.h>

namespace mc {

template<typename T>
struct TransitionTraits
{
    [[nodiscard]] static constexpr auto interpolate(T a, T b, double t) -> T
    {
        return std::lerp(a, b, static_cast<T>(t));
    }
};

template<>
struct TransitionTraits<juce::Colour>
{
    [[nodiscard]] static auto interpolate(juce::Colour a, juce::Colour b, double t) -> juce::Colour
    {
        return a.interpolatedWith(b, static_cast<float>(t));
    }
};

template<typename T>
struct TransitionTraits<juce::Rectangle<T>>
{
    [[nodiscard]] static auto
    interpolate(juce::Rectangle<T> const& a, juce::Rectangle<T> const& b, double t)
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
            using Double = TransitionTraits<juce::Rectangle<double>>;
            return Double::interpolate(a.toDouble(), b.toDouble(), t).template toType<T>();
        }
    }
};

}  // namespace mc
