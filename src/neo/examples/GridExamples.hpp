#pragma once

#include "neo/animations/Animation.hpp"
#include "neo/components/BannerCarousel.hpp"

namespace neo {

struct GridExamples final : juce::Component
{
    GridExamples();
    ~GridExamples() override = default;

    auto layout() -> void;
    auto resized() -> void override;

private:
    [[nodiscard]] static auto makeAnimation() -> Animation::Spec
    {
        return {
            .duration       = std::chrono::milliseconds{600},
            .delay          = std::chrono::milliseconds{0},
            .timingFunction = TimingFunction::EaseInOut,
        };
    }

    Animation _animation{this, makeAnimation()};
    AnimationProperty<float> _transition{_animation};

    bool _isVertical{false};
    std::array<juce::Rectangle<int>, 5> _current{};
    std::array<juce::Rectangle<int>, 5> _next{};
    std::array<Thumbnail, 5> _thumbnails{
        Thumbnail{  "ONE", juce::Colours::yellow, juce::Colours::green},
        Thumbnail{  "TWO",   juce::Colours::blue, juce::Colours::green},
        Thumbnail{"THREE",    juce::Colours::red, juce::Colours::green},
        Thumbnail{ "FOUR",  juce::Colours::white, juce::Colours::green},
        Thumbnail{ "FIVE", juce::Colours::purple, juce::Colours::green},
    };
};
}  // namespace neo
