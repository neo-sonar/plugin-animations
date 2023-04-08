#pragma once

#include <juce_graphics/juce_graphics.h>

namespace mc {

struct TrimPathEffect
{
    TrimPathEffect() = default;
    explicit TrimPathEffect(float start, float end = 1.0F);

    auto start(float newStart) -> void;
    auto end(float newEnd) -> void;

    [[nodiscard]] auto operator()(juce::Path const& path) -> juce::Path;

private:
    float _start{0};
    float _end{0};
};

}  // namespace mc
