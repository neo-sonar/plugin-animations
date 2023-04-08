#pragma once

#include "AnimatedButton.hpp"
#include "LoaderCarousel.hpp"

struct MainComponent final
    : juce::Component
    , juce::Timer
{
    MainComponent();
    ~MainComponent() override = default;

    auto paint(juce::Graphics& g) -> void override;
    auto resized() -> void override;

    auto timerCallback() -> void override;

private:
    mc::AnimatedButton _play{"Play"};
    mc::LoaderCarousel _loader{};
    juce::ComboBox _transition{"Transition"};
    juce::Slider _duration{juce::Slider::LinearHorizontal, juce::Slider::TextBoxRight};

    juce::Rectangle<int> _canvas{};
    juce::Time _startTime{};
};
