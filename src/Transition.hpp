#pragma once

#include "CubicInterpolation.hpp"
#include "TransitionProperty.hpp"
#include "TransitionTimer.hpp"

#include <juce_gui_basics/juce_gui_basics.h>

namespace mc {

struct TransitionSpec
{
    std::chrono::milliseconds duration{600};
    std::chrono::milliseconds delay{0};
    bool isLooping{false};
    std::function<double(double)> timingFunction{TimingFunction::Linear};
};

template<typename T>
struct Transition
{
    explicit Transition(juce::Component* parent, TransitionSpec const& spec = {})
        : _parent{parent}
        , _spec{spec}
    {
        jassert(_parent != nullptr);

        duration(_spec.duration);
        delay(_spec.delay);

        _timer.onTick = [this] { _parent->repaint(); };
    }

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
    juce::Component* _parent{nullptr};
    TransitionSpec _spec;
    TransitionTimer _timer{_parent, _spec.isLooping};
    TransitionProperty<T> _property;
};

}  // namespace mc
