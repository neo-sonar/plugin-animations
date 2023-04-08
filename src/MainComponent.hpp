#pragma once

#include "AnimatedButton.hpp"
#include "BannerCarousel.hpp"
#include "LoaderCarousel.hpp"
#include "TabSelector.hpp"

namespace mc {
struct PathExamples final : juce::Component
{
    PathExamples();
    ~PathExamples() override = default;

    auto paint(juce::Graphics& g) -> void override;

private:
    [[nodiscard]] static auto makeAnimationSpec(juce::Component* comp) -> AnimationSpec
    {
        return {
            .parent         = comp,
            .trigger        = AnimationTriggerType::Manual,
            .transitionTime = Milliseconds<int>{2000},
            .isLooping      = true,
        };
    }

    AnimatedProperty<float> _trim{makeAnimationSpec(this), 0.0F, 1.0F};
};

struct TransitionExamples final : juce::Component
{
    TransitionExamples();
    ~TransitionExamples() override = default;

    auto paint(juce::Graphics& g) -> void override;
    auto resized() -> void override;

private:
    [[nodiscard]] static auto makeAnimationSpec(juce::Component* comp) -> AnimationSpec
    {
        return {
            .parent         = comp,
            .trigger        = AnimationTriggerType::Manual,
            .transitionTime = Milliseconds<int>{2000},
            .isLooping      = true,
        };
    }

    AnimatedProperty<float> _trim{makeAnimationSpec(this), 0.0F, 1.0F};
    juce::ComboBox _transition{"Transition"};
    juce::Slider _duration{juce::Slider::LinearHorizontal, juce::Slider::TextBoxRight};
    juce::Rectangle<float> _canvas{};
};

struct WidgetsExamples final : juce::Component
{
    WidgetsExamples();
    ~WidgetsExamples() override = default;

    auto paint(juce::Graphics& g) -> void override;
    auto resized() -> void override;

private:
    mc::AnimatedButton _play{"Play"};
    mc::LoaderCarousel<3> _loader3{};
    mc::LoaderCarousel<4> _loader4{};
    mc::LoaderCarousel<5> _loader5{};
    mc::LoaderCarousel<6> _loader6{};
    mc::LoaderCarousel<7> _loader7{};
};

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
    mc::BannerCarousel _banners{};
};
}  // namespace mc

struct MainComponent final : juce::Component
{
    MainComponent();
    ~MainComponent() override = default;

    auto paint(juce::Graphics& g) -> void override;
    auto resized() -> void override;

private:
    juce::TextButton _pathToggle{"path"};
    juce::TextButton _transitionToggle{"transition"};
    juce::TextButton _widgetsToggle{"widgets"};
    juce::TextButton _bannersToggle{"banners"};

    mc::PathExamples _path{};
    mc::TransitionExamples _transition{};
    mc::WidgetsExamples _widgets{};
    mc::BannerExamples _banners{};

    mc::TabSelector _tabs;
};
