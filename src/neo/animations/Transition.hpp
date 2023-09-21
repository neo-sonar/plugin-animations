#pragma once

#include "neo/animations/KeyframeProperty.hpp"
#include "neo/animations/KeyframeTimer.hpp"
#include "neo/animations/TimingFunction.hpp"

#include <juce_gui_basics/juce_gui_basics.h>

#include <span>

namespace neo {

struct Transition
{
    struct Spec
    {
        std::chrono::milliseconds duration{600};
        std::chrono::milliseconds delay{0};
        std::function<double(double)> timingFunction{TimingFunction::Linear};
    };

    Transition(juce::Component* parent, Spec spec);

    auto forward() -> void;
    auto backward() -> void;

    auto setDelay(std::chrono::milliseconds ms) -> void;
    auto setDuration(std::chrono::milliseconds ms) -> void;

    [[nodiscard]] auto getPosition() const -> double;

    std::function<void()> onTick;

private:
    juce::Component* _parent{nullptr};
    Spec _spec;
    KeyframeTimer _timer{_parent, false};
};

template<typename T, typename Interpolator = KeyframeInterpolator<T>>
struct TransitionProperty : KeyframeProperty<T, Interpolator>
{
    explicit TransitionProperty(Transition& transition) : TransitionProperty{transition, {}, {}} {}

    TransitionProperty(Transition& transition, T const& from, T const& to)
        : KeyframeProperty<T, Interpolator>{from, to}
        , _transition{transition}
    {}

    [[nodiscard]] auto get() const -> T { return KeyframeProperty<T, Interpolator>::get(_transition.getPosition()); }

private:
    Transition& _transition;
};

}  // namespace neo
