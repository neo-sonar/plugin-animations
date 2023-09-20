#pragma once

#include "neo/animations/TimingFunction.hpp"
#include "neo/components/AnimatedButton.hpp"

namespace mc {

struct Thumbnail final : juce::Component
{
    Thumbnail(juce::String const& text, juce::Colour colorTop, juce::Colour colorBottom)
        : _text{text}
        , _colorTop{colorTop}
        , _colorBottom{colorBottom}
    {}

    ~Thumbnail() override = default;

    auto paint(juce::Graphics& g) -> void override
    {
        g.setGradientFill(_gradient);
        g.fillRoundedRectangle(getLocalBounds().toFloat(), 4.0F);

        g.setFont(32.0F);
        g.setColour(juce::Colours::black);
        g.drawText(_text, getLocalBounds().toFloat(), juce::Justification::centred);
    }

    auto resized() -> void override
    {
        _gradient = juce::ColourGradient::vertical(_colorTop, _colorBottom, getLocalBounds());
    }

private:
    juce::String _text;
    juce::ColourGradient _gradient;
    juce::Colour _colorTop;
    juce::Colour _colorBottom;
};

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
        Content()
        {
            for (auto& thumbnail : _thumbnails) {
                addAndMakeVisible(thumbnail);
            }
            _move.keyframes(0.0F, 1.0F);
            _animation.onTick = [this] { layout(); };
        }

        ~Content() override = default;

        auto reset() -> void { resized(); }

        auto prev() -> void
        {
            _current = _next;
            std::rotate(_next.begin(), std::next(_next.begin()), _next.end());
            _animation.play();
        }

        auto next() -> void
        {
            _current = _next;
            std::rotate(_next.rbegin(), std::next(_next.rbegin()), _next.rend());
            _animation.play();
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

            _current[0] = proxies[0].getBounds();
            _current[1] = proxies[1].getBounds();
            _current[2] = proxies[2].getBounds();
            _current[3] = proxies[3].getBounds();
            _current[4] = proxies[4].getBounds();

            _next[0] = proxies[0].getBounds();
            _next[1] = proxies[1].getBounds();
            _next[2] = proxies[2].getBounds();
            _next[3] = proxies[3].getBounds();
            _next[4] = proxies[4].getBounds();
            layout();
        }

        auto layout() -> void
        {
            using Traits = KeyframeTraits<juce::Rectangle<int>>;

            auto const pos = static_cast<double>(_move.get());
            _thumbnails[0].setBounds(Traits::interpolate(_current[0], _next[0], pos));
            _thumbnails[1].setBounds(Traits::interpolate(_current[1], _next[1], pos));
            _thumbnails[2].setBounds(Traits::interpolate(_current[2], _next[2], pos));
            _thumbnails[3].setBounds(Traits::interpolate(_current[3], _next[3], pos));
            _thumbnails[4].setBounds(Traits::interpolate(_current[4], _next[4], pos));
        }

    private:
        static auto makeAnimation() -> AnimationSpec
        {
            return {
                .duration       = std::chrono::milliseconds{600},
                .delay          = std::chrono::milliseconds{0},
                .timingFunction = TimingFunction::EaseInOutBack,
            };
        }

        Animation _animation{this, makeAnimation()};
        AnimationProperty<float> _move{_animation};

        std::array<juce::Rectangle<int>, 5> _current;
        std::array<juce::Rectangle<int>, 5> _next;
        std::array<Thumbnail, 5> _thumbnails{
            Thumbnail{  "ONE", juce::Colours::transparentBlack, juce::Colours::transparentBlack},
            Thumbnail{  "TWO",           juce::Colours::yellow,            juce::Colours::green},
            Thumbnail{"THREE",           juce::Colours::yellow,            juce::Colours::green},
            Thumbnail{ "FOUR",           juce::Colours::yellow,            juce::Colours::green},
            Thumbnail{ "FIVE",           juce::Colours::yellow,            juce::Colours::green},
        };
    };

    Content _content;
    juce::Viewport _viewport;
};

}  // namespace mc
