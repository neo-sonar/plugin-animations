#pragma once

#include "AnimationDirection.hpp"

#include <boost/sml.hpp>
#include <juce_gui_basics/juce_gui_basics.h>

namespace mc {

struct AnimationTimer
{
    explicit AnimationTimer(juce::Component* parent, bool isLooping = false);

    [[nodiscard]] auto position() const -> double;

    auto duration(std::chrono::milliseconds ms) -> void;
    auto delay(std::chrono::milliseconds ms) -> void;

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

namespace v2 {

struct Spec
{
    bool isLooping{false};
    std::chrono::milliseconds delay{0};
    std::chrono::milliseconds duration{1000};
    std::function<void()> onTick{};
};

// states
struct Idle
{};

struct Waiting
{
    std::chrono::system_clock::time_point start{};
};

struct Running
{
    std::chrono::system_clock::time_point start{};
    std::chrono::system_clock::time_point now{};
};

struct Paused
{
    std::chrono::system_clock::time_point start{};
};

struct Stopped
{};

// events
struct Play
{
    std::chrono::system_clock::time_point now{};
    AnimationDirection direction{AnimationDirection::normal};
};

struct Pause
{};

struct Tick
{
    std::chrono::system_clock::time_point now{};
};

struct StateMachine
{
    auto operator()() const noexcept
    {
        using namespace boost::sml;

        auto startWaiting = [](Waiting& w, Play const& play) { w.start = play.now; };
        auto startRunning = [](Tick const& tick, Running& r) { r.start = tick.now; };

        auto updatePosition = [](Spec& spec, Tick const& tick, Running& running) {
            running.now = tick.now;

            if (spec.isLooping) {
                auto const delta = tick.now - running.start;
                if (delta >= spec.duration) {
                    auto const wrapAround = delta - spec.duration;
                    running.start         = tick.now - wrapAround;
                }
            }

            spec.onTick();
        };

        auto waitTimeRemaining = [](Spec const& spec, Tick const& tick, Waiting const& waiting) {
            auto const delta = tick.now - waiting.start;
            return delta < spec.delay;
        };

        auto runTimeRemaining = [](Spec const& spec, Tick const& tick, Running const& running) {
            if (spec.isLooping) {
                return true;
            }
            auto const delta = tick.now - running.start;
            return delta < spec.duration;
        };

        // clang-format off
            return make_transition_table(
               *state<Idle>     + event<Play>                        / startWaiting   = state<Waiting>,
                state<Waiting>  + event<Tick>[waitTimeRemaining]                                      ,
                state<Waiting>  + event<Tick>[!waitTimeRemaining]    / startRunning   = state<Running>,
                state<Running>  + event<Tick>[runTimeRemaining]      / updatePosition                 ,
                state<Running>  + event<Tick>[!runTimeRemaining]                      = X
            );
        // clang-format on
    }
};

struct Timer
{
    explicit Timer(juce::Component* parent, bool isLooping = false) : _spec{.isLooping = isLooping}, _parent{parent}
    {
        _spec.onTick = [this] {
            jassert(this->onTick);
            this->onTick();
        };
    }

    [[nodiscard]] auto position() const -> double
    {
        if (!_sm) {
            return 0.0;
        }
        using namespace boost::sml;

        if (_sm->is(X)) {
            return 1.0;
        }
        if (_sm->is(state<Idle>)) {
            return 0.0;
        }
        if (_sm->is(state<Waiting>)) {
            return 0.0;
        }

        if (_sm->is(state<Running>)) {
            namespace chrono = std::chrono;
            return chrono::duration_cast<chrono::duration<double>>(_running.now - _running.start)
                 / chrono::duration_cast<chrono::duration<double>>(_spec.duration);
        }

        jassertfalse;
        return 0.0;
    }

    auto duration(std::chrono::milliseconds ms) -> void { _spec.duration = ms; }

    auto delay(std::chrono::milliseconds ms) -> void { _spec.delay = ms; }

    auto play(AnimationDirection dir) -> void
    {
        _sm = std::make_unique<boost::sml::sm<StateMachine>>(_spec, _waiting, _running, _paused);
        _sm->process_event(Play{
            .now       = std::chrono::system_clock::now(),
            .direction = dir,
        });
    }

    std::function<void()> onTick;

private:
    auto tick() -> void
    {
        if (!_sm) {
            return;
        }
        _sm->process_event(Tick{.now = std::chrono::system_clock::now()});
    }

    Spec _spec{};
    Waiting _waiting{};
    Running _running{};
    Paused _paused{};
    std::unique_ptr<boost::sml::sm<StateMachine>> _sm;

    juce::Component* _parent;
    juce::VBlankAttachment _vblankAttachment{_parent, [this] { tick(); }};
};

}  // namespace v2

}  // namespace mc
