#pragma once

#include "AnimatedButton.hpp"
#include "CubicInterpolation.hpp"

namespace mc {

struct BannerCarousel final : juce::Component
{
    BannerCarousel()
    {
        _viewport.setViewedComponent(&_content, false);
        _viewport.setScrollBarsShown(false, false);

        addAndMakeVisible(_content);
        addAndMakeVisible(_viewport);
    }

    ~BannerCarousel() override = default;

    auto reset() -> void { _content.reset(); }

    auto prev() -> void { _content.prev(); }

    auto next() -> void { _content.next(); }

    auto resized() -> void override
    {
        _viewport.setBounds(getLocalBounds());
        _viewport.getViewedComponent()->setBounds(
            _viewport.getLocalBounds().expanded(_viewport.getLocalBounds().proportionOfWidth(0.33), 0)
        );
    }

private:
    struct Content final : juce::Component
    {
        Content() : _move{makeTransition(), 0.0F, 1.0F} {}

        ~Content() override = default;

        auto reset() -> void { resized(); }

        auto prev() -> void
        {
            _current = _next;
            std::rotate(_next.begin(), std::next(_next.begin()), _next.end());
            _move.trigger();
        }

        auto next() -> void
        {
            _current = _next;
            std::rotate(_next.rbegin(), std::next(_next.rbegin()), _next.rend());
            _move.trigger();
        }

        auto paint(juce::Graphics& g) -> void override
        {
            using Interpolator = AnimatedPropertyInterpolator<juce::Rectangle<float>>;

            auto const t = _move.get();
            g.setColour(juce::Colours::transparentBlack);
            g.fillRect(Interpolator::interpolate(_current[0], _next[0], t));
            g.setColour(juce::Colours::green);
            g.fillRect(Interpolator::interpolate(_current[1], _next[1], t));
            g.setColour(juce::Colours::blue);
            g.fillRect(Interpolator::interpolate(_current[2], _next[2], t));
            g.setColour(juce::Colours::yellow);
            g.fillRect(Interpolator::interpolate(_current[3], _next[3], t));
            g.setColour(juce::Colours::white);
            g.fillRect(Interpolator::interpolate(_current[4], _next[4], t));
        }

        auto resized() -> void override
        {
            using namespace juce;
            using Track = juce::Grid::TrackInfo;

            auto proxies = std::array<juce::Component, 5>{};

            auto grid            = juce::Grid{};
            grid.autoFlow        = Grid::AutoFlow::row;
            grid.templateColumns = {Track{1_fr}, Track{1_fr}, Track{1_fr}, Track{1_fr}, Track{1_fr}};
            grid.templateRows    = {Track{1_fr}};
            grid.autoRows        = Track{1_fr};
            grid.autoColumns     = Track{1_fr};
            grid.rowGap          = 4_px;
            grid.columnGap       = 16_px;
            grid.items           = {
                GridItem{proxies[0]},
                GridItem{proxies[1]},
                GridItem{proxies[2]},
                GridItem{proxies[3]},
                GridItem{proxies[4]},
            };
            grid.performLayout(getLocalBounds());

            _current[0] = proxies[0].getBounds().toFloat();
            _current[1] = proxies[1].getBounds().toFloat();
            _current[2] = proxies[2].getBounds().toFloat();
            _current[3] = proxies[3].getBounds().toFloat();
            _current[4] = proxies[4].getBounds().toFloat();

            _next[0] = proxies[0].getBounds().toFloat();
            _next[1] = proxies[1].getBounds().toFloat();
            _next[2] = proxies[2].getBounds().toFloat();
            _next[3] = proxies[3].getBounds().toFloat();
            _next[4] = proxies[4].getBounds().toFloat();
        }

    private:
        auto makeTransition() -> Transition
        {

            return {
                .parent    = this,
                .trigger   = AnimationTriggerType::Manual,
                .duration  = Milliseconds<int>{800},
                .isLooping = false,
                .ease      = EaseInOutBack,
            };
        }

        AnimatedProperty<float> _move;
        std::array<juce::Rectangle<float>, 5> _current;
        std::array<juce::Rectangle<float>, 5> _next;
    };

    Content _content;
    juce::Viewport _viewport;
};

}  // namespace mc
