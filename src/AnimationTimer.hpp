#pragma once

#include "AnimationDirection.hpp"

// #include <boost/sml.hpp>
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

// namespace v2 {

// struct StateMachineLogger
// {
//     template<class SM, class TEvent>
//     void log_process_event(TEvent const&)
//     {
//         printf(
//             "[%s][process_event] %s\n",
//             boost::sml::aux::get_type_name<SM>(),
//             boost::sml::aux::get_type_name<TEvent>()
//         );
//     }

//     template<class SM, class TGuard, class TEvent>
//     void log_guard(TGuard const&, TEvent const&, bool result)
//     {
//         printf(
//             "[%s][guard] %s %s %s\n",
//             boost::sml::aux::get_type_name<SM>(),
//             boost::sml::aux::get_type_name<TGuard>(),
//             boost::sml::aux::get_type_name<TEvent>(),
//             (result ? "[OK]" : "[Reject]")
//         );
//     }

//     template<class SM, class TAction, class TEvent>
//     void log_action(TAction const&, TEvent const&)
//     {
//         printf(
//             "[%s][action] %s %s\n",
//             boost::sml::aux::get_type_name<SM>(),
//             boost::sml::aux::get_type_name<TAction>(),
//             boost::sml::aux::get_type_name<TEvent>()
//         );
//     }

//     template<class SM, class TSrcState, class TDstState>
//     void log_state_change(TSrcState const& src, TDstState const& dst)
//     {
//         printf(
//             "[%s][transition] %s -> %s\n",
//             boost::sml::aux::get_type_name<SM>(),
//             src.c_str(),
//             dst.c_str()
//         );
//     }
// };

// struct Spec
// {
//     bool isLooping{false};
//     std::chrono::milliseconds delay{0};
//     std::chrono::milliseconds duration{1000};
//     std::function<void()> onTick{};
// };

// // states
// struct Idle
// {};

// struct Waiting
// {
//     std::chrono::system_clock::time_point start{};
// };

// struct Running
// {
//     std::chrono::system_clock::time_point start{};
//     std::chrono::system_clock::time_point now{};
// };

// struct Paused
// {
//     std::chrono::system_clock::time_point start{};
// };

// // events
// struct Play
// {
//     std::chrono::system_clock::time_point now{};
//     AnimationDirection direction{AnimationDirection::normal};
// };

// struct Pause
// {};

// struct Tick
// {
//     std::chrono::system_clock::time_point now{};
// };

// struct StateMachine
// {
//     auto operator()() const noexcept
//     {
//         using namespace boost::sml;

//         auto startWaiting = [](Waiting& w) { w.start = std::chrono::system_clock::now(); };
//         auto startRunning = [](Running& r) { r.start = std::chrono::system_clock::now(); };

//         auto updatePosition = [](Spec& spec, Tick const& tick, Running& running) {
//             // DBG("UPDATE: pos");
//             running.now = tick.now;
//             if (spec.onTick) { spec.onTick(); }
//         };

//         auto waitTimeRemaining = [](Spec const& spec, Tick const& tick, Waiting const& waiting) {
//             auto const delta = tick.now - waiting.start;
//             DBG("waitStart " << std::chrono::duration_cast<std::chrono::milliseconds>(
//                                     waiting.start.time_since_epoch()
//                 )
//                                     .count());
//             // DBG("waitTime " <<
//             // std::chrono::duration_cast<std::chrono::milliseconds>(delta).count()); DBG("waitTime "
//             // << std::chrono::duration_cast<std::chrono::milliseconds>(delta).count());
//             return delta < spec.delay;
//         };

//         auto runTimeRemaining = [](Spec const& spec, Tick const& tick, Running const& running) {
//             auto const delta = tick.now - running.start;
//             DBG("runTime " << std::chrono::duration_cast<std::chrono::milliseconds>(delta).count());
//             return delta < spec.duration;
//         };

//         // clang-format off
//             return make_transition_table(
//                *state<Idle>     + event<Play>                        / startWaiting   = state<Waiting>,
//                 state<Waiting>  + event<Tick>[waitTimeRemaining]                                      ,
//                 state<Waiting>  + event<Tick>[not waitTimeRemaining] / startRunning   = state<Running>,
//                 state<Running>  + event<Tick>[runTimeRemaining]      / updatePosition                 ,
//                 state<Running>  + event<Tick>[not runTimeRemaining]                   = X
//             );
//         // clang-format on
//     }
// };

// template<typename... Ts>
// struct Overload : Ts...
// {
//     using Ts::operator()...;
// };

// template<typename... Ts>
// Overload(Ts...) -> Overload<Ts...>;

// struct Timer
// {
//     explicit Timer(juce::Component* parent, bool isLooping = false)
//         : _spec{.isLooping = isLooping}
//         , _parent{parent}
//     {
//         _spec.onTick = [this] {
//             jassert(this->onTick);
//             this->onTick();
//         };
//     }

//     [[nodiscard]] auto position() const -> double
//     {
//         using namespace boost::sml;

//         if (_sm.is(X)) { return 1.0; }
//         if (_sm.is(state<Idle>)) { return 0.0; }
//         if (_sm.is(state<Waiting>)) { return 0.0; }

//         if (_sm.is(state<Running>)) {
//             auto pos     = 0.0;
//             auto generic = [](auto) {};
//             auto running = [&pos, this](Running running) {
//                 namespace chrono = std::chrono;
//                 auto delta       = running.now - running.start;
//                 auto dur         = chrono::duration_cast<chrono::duration<double>>(_spec.duration);
//                 pos              = chrono::duration_cast<chrono::duration<double>>(delta) / dur;
//             };
//             _sm.visit_current_states(Overload{generic, running});
//             return pos;
//         }

//         jassertfalse;
//         return 0.0;
//     }

//     auto duration(std::chrono::milliseconds ms) -> void { _spec.duration = ms; }

//     auto delay(std::chrono::milliseconds ms) -> void { _spec.delay = ms; }

//     auto play(AnimationDirection dir) -> void { _sm.process_event(Play{.direction = dir}); }

//     std::function<void()> onTick;

// private:
//     auto tick() -> void
//     {
//         _sm.process_event(Tick{
//             .now = std::chrono::system_clock::now(),
//         });
//     }

//     Spec _spec{};
//     StateMachineLogger _logger;
//     boost::sml::sm<StateMachine, boost::sml::logger<StateMachineLogger>> _sm{_spec, _logger};

//     juce::Component* _parent;
//     juce::VBlankAttachment _vblankAttachment{_parent, [this] { tick(); }};
// };

// }  // namespace v2

}  // namespace mc
