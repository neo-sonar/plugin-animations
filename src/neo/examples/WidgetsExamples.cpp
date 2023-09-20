#include "WidgetsExamples.hpp"

namespace neo {

WidgetsExamples::WidgetsExamples()
{
    addAndMakeVisible(_play);
    addAndMakeVisible(_loader3);
    addAndMakeVisible(_loader4);
    addAndMakeVisible(_loader5);
    addAndMakeVisible(_loader6);
    addAndMakeVisible(_loader7);
}

auto WidgetsExamples::paint(juce::Graphics& g) -> void { g.fillAll(juce::Colours::black); }

auto WidgetsExamples::resized() -> void
{
    using namespace juce;
    using Track = juce::Grid::TrackInfo;

    auto grid            = juce::Grid{};
    grid.autoFlow        = Grid::AutoFlow::column;
    grid.templateColumns = {Track{1_fr}, Track{1_fr}, Track{1_fr}};
    grid.templateRows    = {Track{1_fr}, Track{1_fr}, Track{1_fr}};
    grid.autoRows        = Track{1_fr};
    grid.autoColumns     = Track{1_fr};
    grid.rowGap          = 4_px;
    grid.columnGap       = 4_px;
    grid.items           = {
        GridItem{_play},
        GridItem{_loader3},
        GridItem{_loader4},
        GridItem{_loader5},
        GridItem{_loader6},
        GridItem{_loader7},
    };

    grid.performLayout(getLocalBounds().reduced(8));
}

}  // namespace neo
