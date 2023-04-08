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
struct AnimatedPropertyInterpolator<juce::Rectangle<T>>
{
    static auto interpolate(juce::Rectangle<T> a, juce::Rectangle<T> b, double t)
        -> juce::Rectangle<T>
    {
        return [](juce::Rectangle<double> aa, juce::Rectangle<double> bb, double ts) {
            return juce::Rectangle<double>{
                std::lerp(aa.getX(), bb.getX(), ts),
                std::lerp(aa.getY(), bb.getY(), ts),
                std::lerp(aa.getWidth(), bb.getWidth(), ts),
                std::lerp(aa.getHeight(), bb.getHeight(), ts),
            };
        }(a.toDouble(), b.toDouble(), t)
                   .toType<T>();
    }
};

struct AnimationSpec
{
    juce::Component* parent{nullptr};
    AnimationTriggerType trigger{AnimationTriggerType::Hover};
    Milliseconds<int> transitionTime{400};
    bool isLooping{false};
};

template<typename T>
struct AnimatedProperty
{
    AnimatedProperty(AnimationSpec const& spec, T const& a, T const& b)
        : _spec{spec}
        , _trigger{makeAnimationTrigger(_spec.trigger, _timer, _spec.parent)}
        , _keyframes{a, b}
    {
        jassert(_spec.parent != nullptr);

        duration(_spec.transitionTime);
        _timer.onTick = [this] { _spec.parent->repaint(); };
    }

    AnimatedProperty(juce::Component* parent, T const& a, T const& b)
        : AnimatedProperty{{.parent = parent}, a, b}
    {}

    auto duration(Milliseconds<int> ms)
    {
        _spec.transitionTime = ms;
        _timer.duration(ms);
    }

    auto keyframes(T const& a, T const& b) { _keyframes = {a, b}; }

    [[nodiscard]] auto get() const -> T
    {
        return AnimatedPropertyInterpolator<T>::interpolate(
            _keyframes.front(),
            _keyframes.back(),
            _timer.position()
        );
    }

private:
    AnimationSpec _spec;

    AnimationTimer _timer;
    std::unique_ptr<AnimationTrigger> _trigger;

    std::array<T, 2> _keyframes;
};

using AnimatedColor = AnimatedProperty<juce::Colour>;

}  // namespace mc
