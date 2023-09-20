#include "Animation.hpp"

namespace neo {

Animation::Animation(juce::Component* parent, Spec spec) : _parent{parent}, _spec{std::move(spec)}
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

auto Animation::setDuration(std::chrono::milliseconds ms) -> void
{
    _spec.duration = ms;
    _timer.setDuration(ms);
}

auto Animation::setDelay(std::chrono::milliseconds ms) -> void
{
    _spec.delay = ms;
    _timer.setDelay(ms);
}

auto Animation::play(KeyframeTimer::Direction direction) -> void { _timer.play(direction); }

auto Animation::getPosition() const -> double { return _spec.timingFunction(_timer.getPosition()); }

}  // namespace neo
