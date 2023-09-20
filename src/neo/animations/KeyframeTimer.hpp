#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace neo {

struct KeyframeTimer
{
    enum struct Direction
    {
        normal,
        reverse,
        alternate,
        alternateReverse,
    };

    explicit KeyframeTimer(juce::Component* parent, bool isLooping = false);

    auto play(Direction dir) -> void;

    auto setDuration(std::chrono::milliseconds ms) -> void;
    auto setDelay(std::chrono::milliseconds ms) -> void;

    [[nodiscard]] auto getPosition() const -> double;

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
    Direction _direction{Direction::normal};
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
