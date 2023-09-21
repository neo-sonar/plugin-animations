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

    struct Clock
    {
        using TimePoint = std::chrono::system_clock::time_point;

        Clock()          = default;
        virtual ~Clock() = default;

        [[nodiscard]] virtual auto now() const -> TimePoint = 0;
    };

    explicit KeyframeTimer(juce::Component* parent, bool isLooping = false);

    auto play(Direction dir) -> void;

    auto setDuration(std::chrono::milliseconds ms) -> void;
    auto setDelay(std::chrono::milliseconds ms) -> void;
    auto setClock(std::unique_ptr<Clock> clock) -> void;

    [[nodiscard]] auto getPosition() const -> double;

    std::function<void()> onTick;

private:
    enum struct State : std::uint8_t
    {
        Idle,
        Delay,
        Transition,
    };

    struct SystemClockProvider final : Clock
    {
        SystemClockProvider()           = default;
        ~SystemClockProvider() override = default;

        [[nodiscard]] auto now() const -> Clock::TimePoint override { return std::chrono::system_clock::now(); }
    };

    auto tick() -> void;

    bool _isLooping;
    Direction _direction{Direction::normal};
    State _state{State::Idle};
    double _position{0.0};

    std::chrono::milliseconds _duration{1000};
    std::chrono::milliseconds _delay{0};

    Clock::TimePoint _delayStart{};
    Clock::TimePoint _transitionStart{};
    std::unique_ptr<Clock> _clock{std::make_unique<SystemClockProvider>()};

    juce::Component* _parent;
    std::unique_ptr<juce::VBlankAttachment> _vblankAttachment;
};

}  // namespace neo
