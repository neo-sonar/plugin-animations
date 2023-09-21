#pragma once

#include "neo/animations/Animation.hpp"
#include "neo/components/TabSelector.hpp"
#include "neo/examples/BannerExamples.hpp"
#include "neo/examples/GridExamples.hpp"
#include "neo/examples/PathExamples.hpp"
#include "neo/examples/TimingExamples.hpp"
#include "neo/examples/WidgetsExamples.hpp"

namespace neo {

struct TabButton final : juce::Button
{
    explicit TabButton(juce::String const& name);
    ~TabButton() override = default;

    auto paintButton(juce::Graphics& g, bool isHighlighted, bool isDown) -> void override;
};

}  // namespace neo

struct MainComponent final : juce::Component
{
    MainComponent();
    ~MainComponent() override = default;

    auto paint(juce::Graphics& g) -> void override;
    auto resized() -> void override;

private:
    [[nodiscard]] static auto makeAnimation() -> neo::Animation::Spec
    {
        return {
            .duration       = std::chrono::milliseconds{600},
            .delay          = std::chrono::milliseconds{0},
            .timingFunction = neo::TimingFunction::EaseInOut,
        };
    }

    neo::Animation _animation{this, makeAnimation()};
    neo::AnimationProperty<juce::Rectangle<int>> _expand{_animation};

    juce::Button* _prev{nullptr};
    juce::Button* _next{nullptr};

    neo::TabButton _widgetsToggle{"widgets"};
    neo::TabButton _bannersToggle{"banners"};
    neo::TabButton _gridsToggle{"grids"};
    neo::TabButton _timingToggle{"timing"};
    neo::TabButton _pathToggle{"path"};

    neo::WidgetsExamples _widgets{};
    neo::BannerExamples _banners{};
    neo::GridExamples _grids{};
    neo::TimingExamples _timing{};
    neo::PathExamples _path{};

    neo::TabSelector _tabs;
};
