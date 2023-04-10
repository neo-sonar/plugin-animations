#pragma once

#include "CubicInterpolation.hpp"
#include "TransitionTimer.hpp"
#include "TransitionTraits.hpp"

#include <juce_gui_basics/juce_gui_basics.h>

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

struct TransitionSpec
{
    juce::Component* parent{nullptr};
    std::chrono::milliseconds duration{600};
    std::chrono::milliseconds delay{0};
    bool isLooping{false};
    std::function<double(double)> timingFunction{[](double t) { return t; }};
};

template<typename T>
struct Transition
{
    Transition(TransitionSpec const& spec, T const& a, T const& b) : _spec{spec}, _property{a, b}
    {
        jassert(_spec.parent != nullptr);

        duration(_spec.duration);
        delay(_spec.delay);

        _timer.onTick = [this] { _spec.parent->repaint(); };
    }

    Transition(juce::Component* parent, T const& a, T const& b) : Transition{{.parent = parent}, a, b}
    {}

    auto duration(std::chrono::milliseconds ms)
    {
        _spec.duration = ms;
        _timer.duration(ms);
    }

    auto delay(std::chrono::milliseconds ms)
    {
        _spec.delay = ms;
        _timer.delay(ms);
    }

    auto keyframes(T const& a, T const& b) { _property.keyframes(a, b); }

    auto forward() { _timer.forward(); }

    auto backward() { _timer.backward(); }

    [[nodiscard]] auto get() const -> T
    {
        return _property.get(_spec.timingFunction(_timer.position()));
    }

private:
    TransitionSpec _spec;
    TransitionTimer _timer{_spec.parent, _spec.isLooping};
    TransitionProperty<T> _property;
};

}  // namespace mc
