#pragma once

#include "neo/animations/Animation.hpp"

namespace neo {

struct PathExamples final : juce::Component
{
    PathExamples();
    ~PathExamples() override = default;

    auto paint(juce::Graphics& g) -> void override;

private:
    [[nodiscard]] static auto makeAnimation() -> Animation::Spec
    {
        return {
            .duration  = std::chrono::milliseconds{2000},
            .isLooping = true,
        };
    }

    Animation _animation{this, makeAnimation()};
    AnimationProperty<float> _trim{_animation};
};

}  // namespace neo
