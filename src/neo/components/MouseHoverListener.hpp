#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace neo {

struct MouseHoverListener final : juce::MouseListener
{
    explicit MouseHoverListener(juce::Component* component) : _component{component}
    {
        component->addMouseListener(this, true);
    }

    ~MouseHoverListener() override { _component->removeMouseListener(this); }

    std::function<void()> onHoverEnter{};
    std::function<void()> onHoverExit{};

private:
    auto mouseEnter(juce::MouseEvent const& /*event*/) -> void override
    {
        if (onHoverEnter) {
            onHoverEnter();
        }
    }

    auto mouseExit(juce::MouseEvent const& /*event*/) -> void override
    {
        if (onHoverExit) {
            onHoverExit();
        }
    }

    juce::Component* _component;
};

}  // namespace neo
