#include "AnimationTimer.hpp"

namespace neo {

AnimationTimer::AnimationTimer(juce::Component* parent, bool isLooping) : _isLooping{isLooping}, _parent{parent} {}

auto AnimationTimer::duration(std::chrono::milliseconds ms) -> void { _duration = ms; }

auto AnimationTimer::delay(std::chrono::milliseconds ms) -> void { _delay = ms; }

[[nodiscard]] auto AnimationTimer::position() const -> double { return _position; }

auto AnimationTimer::play(AnimationDirection dir) -> void
{
    _direction  = dir;
    _state      = State::Delay;
    _position   = 0.0;
    _delayStart = std::chrono::system_clock::now();

    if (not _vblankAttachment) {
        _vblankAttachment = std::make_unique<juce::VBlankAttachment>(_parent, [this] { tick(); });
    }
}

auto AnimationTimer::tick() -> void
{
    if (_state == State::Idle) {
        return;
    }

    if (_state == State::Delay) {
        auto const delta = std::chrono::system_clock::now() - _delayStart;
        if (delta >= _delay) {
            _state           = State::Transition;
            _transitionStart = std::chrono::system_clock::now();
        }
        return;
    }

    if (_state == State::Transition) {
        auto now   = std::chrono::system_clock::now();
        auto delta = now - _transitionStart;
        if (delta > _duration) {
            if (_isLooping) {
                delta            = delta - _duration;
                _transitionStart = now - delta;
            } else {
                delta  = _duration;
                _state = State::Idle;
                _vblankAttachment.reset(nullptr);
            }
        }

        _position = std::chrono::duration_cast<std::chrono::duration<double>>(delta)
                  / std::chrono::duration_cast<std::chrono::duration<double>>(_duration);

        if (_direction == AnimationDirection::reverse) {
            _position = 1.0 - _position;
        }
        if (onTick) {
            onTick();
        }
        return;
    }

    jassertfalse;
}

}  // namespace neo
