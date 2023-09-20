#pragma once

#include "neo/animations/AnimationDirection.hpp"

#include <juce_gui_basics/juce_gui_basics.h>

namespace neo {

struct AnimationTimer
{
    explicit AnimationTimer(juce::Component* parent, bool isLooping = false);

    [[nodiscard]] auto getPosition() const -> double;

    auto setDuration(std::chrono::milliseconds ms) -> void;
    auto setDelay(std::chrono::milliseconds ms) -> void;

    auto play(AnimationDirection dir) -> void;

    std::function<void()> onTick;

private:
    enum struct State : std::uint8_t
    {
        Idle,
        Delay,
        Transition,
    };

    auto tick() -> void;

    bool _isLooping;
    AnimationDirection _direction{AnimationDirection::normal};
    State _state{State::Idle};
    double _position{0.0};

    std::chrono::milliseconds _duration{1000};
    std::chrono::milliseconds _delay{0};

    std::chrono::system_clock::time_point _delayStart{};
    std::chrono::system_clock::time_point _transitionStart{};

    juce::Component* _parent;
    std::unique_ptr<juce::VBlankAttachment> _vblankAttachment;
};

}  // namespace neo
