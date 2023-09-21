#include "TimingExamples.hpp"

namespace neo {

TimingExamples::TimingExamples()
{
    auto names = juce::StringArray{};
    for (auto const& [key, value] : _functions) {
        names.add(key);
    }
    names.sort(true);

    _timingFunctions.addItemList(names, 1);
    _timingFunctions.onChange = [this] { repaint(); };
    addAndMakeVisible(_timingFunctions);
}

auto TimingExamples::paint(juce::Graphics& g) -> void
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.fillRect(_paintArea);

    auto id = _timingFunctions.getText().toStdString();
    if (not _functions.contains(id)) {
        return;
    }

    auto func = _functions.at(id);
    auto path = juce::Path{};
    path.startNewSubPath(0.0F, 0.0F);
    for (auto i{0}; i <= 128; ++i) {
        auto const x = double(i) / 128.0;
        auto const y = func(x);
        path.lineTo(juce::Point{x, y}.toFloat());
    }

    auto transform = path.getTransformToScaleToFit(_paintArea.toFloat(), true);
    g.setColour(juce::Colours::black);
    g.strokePath(path, juce::PathStrokeType{2.0F}, transform);
}

auto TimingExamples::resized() -> void
{
    using namespace juce;
    using Track = juce::Grid::TrackInfo;

    auto proxy           = juce::Component{};
    auto grid            = juce::Grid{};
    grid.autoFlow        = Grid::AutoFlow::column;
    grid.templateColumns = {Track{1_fr}};
    grid.templateRows    = {Track{1_fr}, Track{7_fr}};
    grid.autoRows        = Track{1_fr};
    grid.autoColumns     = Track{1_fr};
    grid.rowGap          = 4_px;
    grid.columnGap       = 4_px;
    grid.items           = {
        GridItem{_timingFunctions},
        GridItem{proxy},
    };

    grid.performLayout(getLocalBounds().reduced(8));

    _paintArea = proxy.getBounds();
}

}  // namespace neo
