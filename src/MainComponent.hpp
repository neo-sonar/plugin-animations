#pragma once

#include "AnimatedButton.hpp"
#include "BannerCarousel.hpp"
#include "LoaderCarousel.hpp"
#include "TabSelector.hpp"
#include "animation/TransitionProperty.hpp"

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

    Animation _animation{this, makeAnimation()};
    AnimationProperty<float> _trim{_animation};
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

    Transition _tran{this, makeTransition()};
    TransitionProperty<float> _trim{_tran};

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
    AnimatedButton _play{"Play"};
    LoaderCarousel<3> _loader3{};
    LoaderCarousel<4> _loader4{};
    LoaderCarousel<5> _loader5{};
    LoaderCarousel<6> _loader6{};
    LoaderCarousel<7> _loader7{};
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
    BannerCarousel _banners{};
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

struct TabButton final : juce::Button
{
    explicit TabButton(juce::String const& name);
    ~TabButton() override = default;

    auto paintButton(juce::Graphics& g, bool isHighlighted, bool isDown) -> void override;
};

}  // namespace mc

struct MainComponent final : juce::Component
{
    MainComponent();
    ~MainComponent() override = default;

    auto paint(juce::Graphics& g) -> void override;
    auto resized() -> void override;

private:
    static auto makeAnimation() -> mc::AnimationSpec
    {
        return {
            .duration       = std::chrono::milliseconds{600},
            .delay          = std::chrono::milliseconds{0},
            .timingFunction = mc::TimingFunction::EaseInOut,
        };
    }

    mc::Animation _animation{this, makeAnimation()};
    mc::AnimationProperty<juce::Rectangle<int>> _expand{_animation};

    juce::Button* _prev{nullptr};
    juce::Button* _next{nullptr};

    mc::TabButton _pathToggle{"path"};
    mc::TabButton _transitionToggle{"transition"};
    mc::TabButton _widgetsToggle{"widgets"};
    mc::TabButton _bannersToggle{"banners"};
    mc::TabButton _gridsToggle{"grids"};

    mc::PathExamples _path{};
    mc::TransitionExamples _transition{};
    mc::WidgetsExamples _widgets{};
    mc::BannerExamples _banners{};
    mc::GridExamples _grids{};

    mc::TabSelector _tabs;
};
