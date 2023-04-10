#pragma once

#include "AnimatedButton.hpp"
#include "Animation.hpp"
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
    [[nodiscard]] static auto makeAnimation() -> AnimationSpec
    {
        return {
            .duration  = std::chrono::milliseconds{2000},
            .isLooping = true,
        };
    }

    Animation<float> _trim{this, makeAnimation()};
};

struct TransitionExamples final : juce::Component
{
    TransitionExamples();
    ~TransitionExamples() override = default;

    auto paint(juce::Graphics& g) -> void override;
    auto resized() -> void override;

private:
    [[nodiscard]] static auto makeTransition() -> TransitionSpec
    {
        return {.duration = std::chrono::milliseconds{2000}};
    }

    Transition<float> _trim{this, makeTransition()};
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

struct GridExamples final : juce::Component
{
    GridExamples();
    ~GridExamples() override = default;

    auto layout() -> void;
    auto resized() -> void override;

private:
    static auto makeAnimation() -> AnimationSpec
    {
        return {
            .duration       = std::chrono::milliseconds{800},
            .delay          = std::chrono::milliseconds{0},
            .timingFunction = TimingFunction::EaseInOut,
        };
    }

    Animation<float> _transition{this, makeAnimation()};
    bool _isVertical{false};
    std::array<juce::Rectangle<int>, 5> _current{};
    std::array<juce::Rectangle<int>, 5> _next{};
    std::array<Thumbnail, 5> _thumbnails{
        Thumbnail{  "ONE", juce::Colours::yellow, juce::Colours::green},
        Thumbnail{  "TWO", juce::Colours::yellow, juce::Colours::green},
        Thumbnail{"THREE", juce::Colours::yellow, juce::Colours::green},
        Thumbnail{ "FOUR", juce::Colours::yellow, juce::Colours::green},
        Thumbnail{ "FIVE", juce::Colours::yellow, juce::Colours::green},
    };
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
    juce::TextButton _gridsToggle{"grids"};

    mc::PathExamples _path{};
    mc::TransitionExamples _transition{};
    mc::WidgetsExamples _widgets{};
    mc::BannerExamples _banners{};
    mc::GridExamples _grids{};

    mc::TabSelector _tabs;
};
