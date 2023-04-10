#pragma once

#include "TimingFunction.hpp"
#include "TransitionProperty.hpp"
#include "AnimationTimer.hpp"

#include <juce_gui_basics/juce_gui_basics.h>

namespace mc {

struct TransitionSpec
{
    std::chrono::milliseconds duration{600};
    std::chrono::milliseconds delay{0};
    std::function<double(double)> timingFunction{TimingFunction::Linear};
};

template<typename... Ts>
struct Transition
{
    explicit Transition(juce::Component* parent, TransitionSpec const& spec = {})
        : _parent{parent}
        , _spec{spec}
    {
        jassert(_parent != nullptr);

        duration(_spec.duration);
        delay(_spec.delay);

        onTick        = [this] { _parent->repaint(); };
        _timer.onTick = [this] {
            if (this->onTick) { this->onTick(); }
        };
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

    template<int I>
    auto keyframes(auto const& a, auto const& b)
    {
        std::get<I>(_props).keyframes(a, b);
    }

    auto forward() { _timer.forward(); }

    auto backward() { _timer.backward(); }

    template<int I>
    [[nodiscard]] auto get() const
    {
        return std::get<I>(_props).get(_spec.timingFunction(_timer.position()));
    }

    std::function<void()> onTick;

private:
    juce::Component* _parent{nullptr};
    TransitionSpec _spec;
    AnimationTimer _timer{_parent, false};
    std::tuple<TransitionProperty<Ts>...> _props;
};

}  // namespace mc
