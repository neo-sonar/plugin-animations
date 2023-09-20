#pragma once

#include "AnimatedButton.hpp"
#include "neo/animations/Animation.hpp"

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

    LoaderCarousel() { _animation.play(); }

    ~LoaderCarousel() override = default;

    auto paint(juce::Graphics& g) -> void override
    {
        auto const scale     = _scale.get();
        auto const translate = _translate.get();
        auto const width     = _points.front().proportionOfWidth(0.5F);

        g.setColour(juce::Colours::white);

        g.fillEllipse(_points.front().reduced(width * scale));
        g.fillEllipse(_points.back().reduced(width * (1.0F - scale)));

        for (auto const& p : std::span<juce::Rectangle<float>>{_points}.subspan(0, NumPoints - 1)) {
            g.fillEllipse(p.translated(translate, 0.0F));
        }
    }

    auto resized() -> void override
    {
        auto area        = getLocalBounds().toFloat();
        auto const width = area.getWidth() / static_cast<float>(_points.size());
        for (auto& p : _points) {
            p = makeSquare(area.removeFromLeft(width)).reduced(2.0F);
        }

        auto const last       = _points.back();
        auto const beforeLast = std::prev(_points.end(), 2);
        auto const distance   = last.getCentreX() - beforeLast->getCentreX();
        _translate.keyframes(0.0F, distance);
    }

private:
    static auto makeAnimation() -> AnimationSpec
    {
        return {
            .duration       = std::chrono::milliseconds{600},
            .isLooping      = true,
            .timingFunction = TimingFunction::EaseInOutBack,
        };
    }

    Animation _animation{this, makeAnimation()};
    AnimationProperty<float> _scale{_animation, 1.0F, 0.0F};
    AnimationProperty<float> _translate{_animation, 0.0F, 24.0F};
    std::array<juce::Rectangle<float>, NumPoints> _points{};
};

}  // namespace mc
