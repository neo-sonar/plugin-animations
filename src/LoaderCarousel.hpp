#pragma once

#include "AnimatedButton.hpp"

namespace mc {

inline constexpr auto makeSquare = []<typename T>(juce::Rectangle<T> const& rect) {
    auto const size = std::min(rect.getWidth(), rect.getHeight());
    return rect.withSizeKeepingCentre(size, size);
};

struct LoaderCarousel final : juce::Component
{
    LoaderCarousel()
        : _scale1{makeAnimationSpec(), 1.0F, 0.0F}
        , _translate2{makeAnimationSpec(), 0.0F, 24.0F}
        , _scale3{makeAnimationSpec(), 0.0F, 1.0F}
    {
        _scale1.trigger();
        _translate2.trigger();
        _scale3.trigger();
    }

    ~LoaderCarousel() override = default;

    auto paint(juce::Graphics& g) -> void override
    {
        auto e1 = _ellipses[0];
        auto e2 = _ellipses[1];
        auto e3 = _ellipses[2];

        g.setColour(juce::Colours::white);
        g.fillEllipse(e1.reduced(e1.proportionOfWidth(0.5F) * _scale1.get()));

        g.fillEllipse(e1.translated(_translate2.get(), 0.0F));
        g.fillEllipse(e2.translated(_translate2.get(), 0.0F));

        g.fillEllipse(e3.reduced(e3.proportionOfWidth(0.5F) * _scale3.get()));
    }

    auto resized() -> void override
    {

        auto area        = getLocalBounds().toFloat();
        auto const width = area.getWidth() / 3.0F;

        _ellipses = {
            makeSquare(area.removeFromLeft(width)).reduced(2.0F),
            makeSquare(area.removeFromLeft(width)).reduced(2.0F),
            makeSquare(area).reduced(2.0F),
        };

        _translate2.keyframes(0.0F, _ellipses[2].getCentreX() - _ellipses[1].getCentreX());
    }

private:
    auto makeAnimationSpec() -> AnimationSpec
    {
        return {
            .parent         = this,
            .trigger        = AnimationTriggerType::Manual,
            .transitionTime = Milliseconds<int>{600},
            .isLooping      = true,
        };
    }

    AnimatedProperty<float> _scale1;
    AnimatedProperty<float> _translate2;
    AnimatedProperty<float> _scale3;

    std::array<juce::Rectangle<float>, 3> _ellipses{};
};

}  // namespace mc
