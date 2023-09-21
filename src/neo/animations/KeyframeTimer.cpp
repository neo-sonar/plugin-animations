#include "KeyframeTimer.hpp"

namespace neo {

KeyframeTimer::KeyframeTimer(juce::Component* parent, bool isLooping) : _isLooping{isLooping}, _parent{parent} {}

auto KeyframeTimer::setDuration(std::chrono::milliseconds ms) -> void { _duration = ms; }

auto KeyframeTimer::setDelay(std::chrono::milliseconds ms) -> void { _delay = ms; }

auto KeyframeTimer::setClock(std::unique_ptr<Clock> clock) -> void
{
    if (clock == nullptr) {
        return;
    }
    _clock = std::move(clock);
}

[[nodiscard]] auto KeyframeTimer::getPosition() const -> double { return _position; }

auto KeyframeTimer::play(Direction dir) -> void
{
    if (_state == State::Idle and dir == Direction::normal) {
        _position = 0.0;
    }

    _direction  = dir;
    _state      = State::Delay;
    _delayStart = _clock->now();

    if (not _vblankAttachment) {
        _vblankAttachment = std::make_unique<juce::VBlankAttachment>(_parent, [this] { tick(); });
    }
}

auto KeyframeTimer::tick() -> void
{
    if (_state == State::Idle) {
        return;
    }

    if (_state == State::Delay) {
        auto const delta = _clock->now() - _delayStart;
        if (delta >= _delay) {
            _state           = State::Transition;
            _transitionStart = _clock->now();
        }
        return;
    }

    if (_state == State::Transition) {
        auto now   = _clock->now();
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

        if (_direction == Direction::reverse) {
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
