#pragma once

#include "AnimatedButton.hpp"
#include "LoaderCarousel.hpp"

struct MainComponent final : juce::Component
{
    MainComponent();
    ~MainComponent() override = default;

    auto paint(juce::Graphics& g) -> void override;
    auto resized() -> void override;

private:
    mc::AnimatedButton _play{"Play"};
    mc::LoaderCarousel _loader{};
    juce::ComboBox _transition{"Transition"};
    juce::Slider _duration{juce::Slider::LinearHorizontal, juce::Slider::TextBoxRight};
    mc::AnimatedProperty<float> _trim{
        {
         .parent         = this,
         .trigger        = mc::AnimationTriggerType::Manual,
         .transitionTime = mc::Milliseconds<int>{2000},
         .isLooping      = true,
         },
        0.0F,
        1.0F
    };

    juce::Rectangle<int> _canvas{};
};
