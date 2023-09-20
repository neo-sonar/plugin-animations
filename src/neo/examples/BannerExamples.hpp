#pragma once

#include "neo/components/BannerCarousel.hpp"

namespace neo {

struct BannerExamples final : juce::Component
{
    BannerExamples();
    ~BannerExamples() override = default;

    auto paint(juce::Graphics& g) -> void override;
    auto resized() -> void override;

private:
    juce::TextButton _prev{"prev"};
    juce::TextButton _reset{"reset"};
    juce::TextButton _next{"next"};
    BannerCarousel _banners{};
};

}  // namespace neo
