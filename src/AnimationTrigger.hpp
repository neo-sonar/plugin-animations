#pragma once

#include "AnimationTimer.hpp"
#include <juce_gui_basics/juce_gui_basics.h>

namespace mc {

struct AnimationTrigger
{
    explicit AnimationTrigger(AnimationTimer& timer) : _timer{timer} {}

    virtual ~AnimationTrigger() = default;

    auto triggerUp() -> void { _timer.restart(true); }

    auto triggerDown() -> void { _timer.restart(false); }

    AnimationTimer& _timer;
};

struct AnimationTriggers
{
    struct Hover final
        : AnimationTrigger
        , juce::MouseListener
    {
        explicit Hover(AnimationTimer& timer, juce::Component* component)
            : AnimationTrigger{timer}
            , _component{component}
        {
            component->addMouseListener(this, true);
        }

        ~Hover() override { _component->removeMouseListener(this); }

    private:
        auto mouseEnter(juce::MouseEvent const& /*event*/) -> void override { triggerUp(); }

        auto mouseExit(juce::MouseEvent const& /*event*/) -> void override
        {
            triggerDown();
        }

        juce::Component* _component;
    };
};

enum struct AnimationTriggerType
{
    Manual,
    Hover,
};

[[nodiscard]] inline auto makeAnimationTrigger(
    AnimationTriggerType type,
    AnimationTimer& timer,
    juce::Component* parent
) -> std::unique_ptr<AnimationTrigger>
{
    if (type == AnimationTriggerType::Manual) {
        return std::make_unique<AnimationTrigger>(timer);
    }
    if (type == AnimationTriggerType::Hover) {
        return std::make_unique<AnimationTriggers::Hover>(timer, parent);
    }
    jassertfalse;
    return {};
}

}  // namespace mc
