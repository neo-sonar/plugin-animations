#include "GridExamples.hpp"

namespace neo {

GridExamples::GridExamples()
{
    for (auto& thumbnail : _thumbnails) {
        addAndMakeVisible(thumbnail);
    }
    _transition.setKeyframes(0.0F, 1.0F);
    _animation.onTick = [this] { layout(); };
}

auto GridExamples::layout() -> void
{
    using Traits = KeyframeTraits<juce::Rectangle<int>>;

    auto const pos = static_cast<double>(_transition.get());
    _thumbnails[0].setBounds(Traits::interpolate(_current[0], _next[0], pos));
    _thumbnails[1].setBounds(Traits::interpolate(_current[1], _next[1], pos));
    _thumbnails[2].setBounds(Traits::interpolate(_current[2], _next[2], pos));
    _thumbnails[3].setBounds(Traits::interpolate(_current[3], _next[3], pos));
    _thumbnails[4].setBounds(Traits::interpolate(_current[4], _next[4], pos));
}

auto GridExamples::resized() -> void
{
    using namespace juce;
    using Track = juce::Grid::TrackInfo;

    auto makeTracks = [](auto count) {
        auto tracks = juce::Array<Track>{};
        tracks.resize(count);
        tracks.fill(Track{1_fr});
        return tracks;
    };

    auto grid        = juce::Grid{};
    grid.autoFlow    = Grid::AutoFlow::row;
    grid.autoRows    = Track{1_fr};
    grid.autoColumns = Track{1_fr};
    grid.rowGap      = 8_px;
    grid.columnGap   = 8_px;

    auto proxies            = std::array<juce::Component, 5>{};
    auto const isHorizontal = getWidth() > getHeight();

    if (not isHorizontal) {
        grid.templateColumns = makeTracks(1);
        grid.templateRows    = makeTracks(5);
        for (auto i{0U}; i < _thumbnails.size(); ++i) {
            grid.items.add(GridItem{proxies[i]});
        }
    } else {
        grid.templateColumns = makeTracks(6);
        grid.templateRows    = makeTracks(2);
        grid.items           = {
            GridItem{proxies[0]}.withArea({}, GridItem::Span(3)),
            GridItem{proxies[1]}.withArea({}, GridItem::Span(3)),
            GridItem{proxies[2]}.withArea({}, GridItem::Span(2)),
            GridItem{proxies[3]}.withArea({}, GridItem::Span(2)),
            GridItem{proxies[4]}.withArea({}, GridItem::Span(2)),
        };
    }

    grid.performLayout(getLocalBounds());
    for (auto i{0U}; i < _thumbnails.size(); ++i) {
        _next[i] = proxies[i].getBounds();
    }

    if (_isVertical != isHorizontal) {
        _isVertical = isHorizontal;
        for (auto i{0U}; i < _thumbnails.size(); ++i) {
            _current[i] = _thumbnails[i].getBounds();
        }
        _animation.play();
    } else {
        layout();
    }
}

}  // namespace neo
