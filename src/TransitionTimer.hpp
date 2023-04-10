#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace mc {

struct TransitionTimer
{
    explicit TransitionTimer(juce::Component* parent, bool isLooping = false);

    [[nodiscard]] auto position() const -> double;

    auto duration(std::chrono::milliseconds ms) -> void;
    auto delay(std::chrono::milliseconds ms) -> void;

    auto forward() -> void;
    auto backward() -> void;

    std::function<void()> onTick;

private:
    enum struct Direction : std::uint8_t
    {
        Forward,
        Backward,
    };

    enum struct State : std::uint8_t
    {
        Idle,
        Delay,
        Transition,
    };

    auto start(Direction dir) -> void;
    auto tick() -> void;

    bool _isLooping;
    Direction _direction{Direction::Forward};
    State _state{State::Idle};
    double _position{0.0};

    std::chrono::milliseconds _duration{1000};
    std::chrono::milliseconds _delay{0};

    std::chrono::system_clock::time_point _delayStart{};
    std::chrono::system_clock::time_point _transitionStart{};

    juce::Component* _parent;
    std::unique_ptr<juce::VBlankAttachment> _vblankAttachment;
};

}  // namespace mc
