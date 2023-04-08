#pragma once

#include "AnimatedButton.hpp"

#include <span>

namespace mc {

inline constexpr auto makeSquare = []<typename T>(juce::Rectangle<T> const& rect) {
    auto const size = std::min(rect.getWidth(), rect.getHeight());
    return rect.withSizeKeepingCentre(size, size);
};

template<int NumPoints>
struct LoaderCarousel final : juce::Component
{
    static_assert(NumPoints >= 3);

    LoaderCarousel()
        : _scale1{makeTransition(), 1.0F, 0.0F}
        , _translate2{makeTransition(), 0.0F, 24.0F}
        , _scale3{makeTransition(), 0.0F, 1.0F}
    {
        _scale1.trigger();
        _translate2.trigger();
        _scale3.trigger();
    }

    ~LoaderCarousel() override = default;

    auto paint(juce::Graphics& g) -> void override
    {
        auto points = std::span<juce::Rectangle<float>>{_points}.subspan(0, NumPoints - 1);

        g.setColour(juce::Colours::white);
        g.fillEllipse(points.front().reduced(points.front().proportionOfWidth(0.5F) * _scale1.get()));
        for (auto const& p : points) { g.fillEllipse(p.translated(_translate2.get(), 0.0F)); }
        g.fillEllipse(_points.back().reduced(_points.back().proportionOfWidth(0.5F) * _scale3.get()));
    }

    auto resized() -> void override
    {

        auto area        = getLocalBounds().toFloat();
        auto const width = area.getWidth() / static_cast<float>(_points.size());
        for (auto& p : _points) { p = makeSquare(area.removeFromLeft(width)).reduced(2.0F); }

        _translate2.keyframes(
            0.0F,
            _points.back().getCentreX() - std::prev(_points.end(), 2)->getCentreX()
        );
    }

private:
    auto makeTransition() -> Transition
    {
        return {
            .parent    = this,
            .trigger   = AnimationTriggerType::Manual,
            .duration  = Milliseconds<int>{600},
            .isLooping = true,
            .ease      = EaseInOutBack,
        };
    }

    AnimatedProperty<float> _scale1;
    AnimatedProperty<float> _translate2;
    AnimatedProperty<float> _scale3;

    std::array<juce::Rectangle<float>, NumPoints> _points{};
};

}  // namespace mc
