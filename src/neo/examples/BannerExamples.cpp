#include "BannerExamples.hpp"

namespace neo {

BannerExamples::BannerExamples()
{
    _prev.onClick  = [this] { _banners.prev(); };
    _reset.onClick = [this] { _banners.reset(); };
    _next.onClick  = [this] { _banners.next(); };

    addAndMakeVisible(_prev);
    addAndMakeVisible(_reset);
    addAndMakeVisible(_next);
    addAndMakeVisible(_banners);
}

auto BannerExamples::paint(juce::Graphics& g) -> void { g.fillAll(juce::Colours::black); }

auto BannerExamples::resized() -> void
{
    using namespace juce;
    using Track = juce::Grid::TrackInfo;

    auto grid            = juce::Grid{};
    grid.autoFlow        = Grid::AutoFlow::column;
    grid.templateColumns = {Track{1_fr}, Track{1_fr}, Track{1_fr}};
    grid.templateRows    = {Track{5_fr}, Track{1_fr}};
    grid.autoRows        = Track{1_fr};
    grid.autoColumns     = Track{1_fr};
    grid.rowGap          = 16_px;
    grid.columnGap       = 16_px;
    grid.items           = {
        GridItem{_banners}.withArea({}, GridItem::Span(3)),
        GridItem{_prev},
        GridItem{_reset},
        GridItem{_next},
    };

    grid.performLayout(getLocalBounds().reduced(32));
}

}  // namespace neo
