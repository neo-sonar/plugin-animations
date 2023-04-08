#pragma once

#include "AnimationTimer.hpp"
#include "AnimationTrigger.hpp"
#include "CubicInterpolation.hpp"
#include <juce_gui_basics/juce_gui_basics.h>

namespace mc {

template<typename T>
struct AnimatedPropertyInterpolator
{
    static constexpr auto interpolate(T a, T b, double t) -> T
    {
        return std::lerp(a, b, static_cast<T>(t));
    }
};

template<>
struct AnimatedPropertyInterpolator<juce::Colour>
{
    static auto interpolate(juce::Colour a, juce::Colour b, double t) -> juce::Colour
    {
        return a.interpolatedWith(b, static_cast<float>(t));
    }
};

template<typename T>
struct AnimatedProperty
{
    AnimatedProperty(juce::Component* parent, T const& a, T const& b)
        : _parent{parent}
        , _trigger{std::make_unique<AnimationTriggers::Hover>(_timer, _parent)}
        , _keyframes{a, b}
    {
        _timer.onTick = [this] { _parent->repaint(); };
    }

    auto duration(double ms) { _timer.duration(ms); }

    [[nodiscard]] auto get() const -> T
    {
        return AnimatedPropertyInterpolator<T>::interpolate(
            _keyframes.front(),
            _keyframes.back(),
            _timer.position()
        );
    }

private:
    juce::Component* _parent;

    AnimationTimer _timer;
    std::unique_ptr<AnimationTrigger> _trigger;

    std::array<T, 2> _keyframes;
    double _position{0};
};

using AnimatedColor = AnimatedProperty<juce::Colour>;

}  // namespace mc
