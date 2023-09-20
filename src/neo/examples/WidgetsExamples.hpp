#pragma once

#include "neo/components/AnimatedButton.hpp"
#include "neo/components/LoaderCarousel.hpp"

namespace neo {

struct WidgetsExamples final : juce::Component
{
    WidgetsExamples();
    ~WidgetsExamples() override = default;

    auto paint(juce::Graphics& g) -> void override;
    auto resized() -> void override;

private:
    AnimatedButton _play{"Play"};
    LoaderCarousel<3> _loader3{};
    LoaderCarousel<4> _loader4{};
    LoaderCarousel<5> _loader5{};
    LoaderCarousel<6> _loader6{};
    LoaderCarousel<7> _loader7{};
};

}  // namespace neo
