#pragma once

#include "Time.hpp"
#include <juce_gui_basics/juce_gui_basics.h>

namespace mc {

struct AnimationTimer final : juce::Timer
{
    AnimationTimer()           = default;
    ~AnimationTimer() override = default;

    auto duration(Milliseconds<int> ms) -> void { _duration = ms; }

    [[nodiscard]] auto position() const -> double { return _position; }

    auto restart(bool forward) -> void
    {
        _forward   = forward;
        _startTime = SystemClock::now();
        // _position  = 0.0;
        startTimerHz(30);
    }

    std::function<void()> onTick;

private:
    auto timerCallback() -> void override
    {
        auto delta = SystemClock::now() - _startTime;
        if (delta > _duration) {
            delta = _duration;
            stopTimer();
        }

        _position = std::chrono::duration_cast<Milliseconds<double>>(delta)
                  / std::chrono::duration_cast<Milliseconds<double>>(_duration);

        if (!_forward) { _position = 1.0 - _position; }
        if (onTick) { onTick(); }
    }

    bool _forward{true};
    double _position{0.0};
    Milliseconds<int> _duration{1000};
    SystemClock::time_point _startTime{};
};

}  // namespace mc
