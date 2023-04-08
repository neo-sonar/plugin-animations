#pragma once

#include "AnimationTimer.hpp"
#include <juce_gui_basics/juce_gui_basics.h>

namespace mc {

struct AnimationTrigger
{
    explicit AnimationTrigger(AnimationTimer& timer) : _timer{timer} {}

    virtual ~AnimationTrigger() = default;

protected:
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

}  // namespace mc
