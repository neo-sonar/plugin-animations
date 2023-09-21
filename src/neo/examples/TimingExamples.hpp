#pragma once

#include "neo/components/AnimatedButton.hpp"
#include "neo/components/LoaderCarousel.hpp"

namespace neo {

struct TimingExamples final : juce::Component
{
    TimingExamples();
    ~TimingExamples() override = default;

    auto paint(juce::Graphics& g) -> void override;
    auto resized() -> void override;

private:
    juce::ComboBox _timingFunctions{"Timing Functions"};
    juce::Rectangle<int> _paintArea{};
    std::unordered_map<std::string, std::function<double(double)>> _functions{
        {       "Linear",        TimingFunction::Linear},
        {      "EaseOut",       TimingFunction::EaseOut},
        {    "EaseInOut",     TimingFunction::EaseInOut},
        {"EaseInOutBack", TimingFunction::EaseInOutBack},
    };
};

}  // namespace neo
