#include "Transition.hpp"

namespace neo {

Transition::Transition(juce::Component* parent, Spec spec) : _parent{parent}, _spec{std::move(spec)}
{
    jassert(_parent != nullptr);

    setDuration(_spec.duration);
    setDelay(_spec.delay);

    onTick        = [this] { _parent->repaint(); };
    _timer.onTick = [this] {
        if (this->onTick) {
            this->onTick();
        }
    };
}

auto Transition::forward() -> void { _timer.play(KeyframeTimer::Direction::normal); }

auto Transition::backward() -> void { _timer.play(KeyframeTimer::Direction::reverse); }

auto Transition::getPosition() const -> double { return _spec.timingFunction(_timer.getPosition()); }

auto Transition::setDuration(std::chrono::milliseconds ms) -> void
{
    _spec.duration = ms;
    _timer.setDuration(ms);
}

auto Transition::setDelay(std::chrono::milliseconds ms) -> void
{
    _spec.delay = ms;
    _timer.setDelay(ms);
}

}  // namespace neo
