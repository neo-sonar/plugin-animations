#include "TransitionExamples.hpp"

namespace neo {

TransitionExamples::TransitionExamples()
{
    addAndMakeVisible(_transition);
    addAndMakeVisible(_duration);
    _transition.addItemList(
        {
            "Linear",
            "Ease-Out",
            "Ease-In-Out",
            "Ease-In-Out-Back",
        },
        1
    );

    _trim.setKeyframes(0.0F, 1.0F);

    _duration.onValueChange = [this] {
        auto const ms = std::chrono::milliseconds{static_cast<int>(_duration.getValue())};
        _tran.setDuration(ms);
    };
    _duration.setRange(100.0, 10000.0, 1.0);
    _duration.setValue(2000.0);
}

auto TransitionExamples::paint(juce::Graphics& g) -> void
{
    g.setColour(juce::Colours::black);
    g.fillRect(_canvas);
}

auto TransitionExamples::resized() -> void
{
    using namespace juce;
    using Track = juce::Grid::TrackInfo;

    auto proxy           = juce::Component{"proxy"};
    auto grid            = juce::Grid{};
    grid.autoFlow        = Grid::AutoFlow::row;
    grid.templateColumns = {Track{1_fr}};
    grid.templateRows    = {Track{1_fr}, Track{1_fr}, Track{5_fr}};
    grid.autoRows        = Track{1_fr};
    grid.autoColumns     = Track{1_fr};
    grid.rowGap          = 4_px;
    grid.columnGap       = 4_px;
    grid.items           = {GridItem{_transition}, GridItem{_duration}, GridItem{proxy}};

    grid.performLayout(getLocalBounds().reduced(8));
    _canvas = proxy.getBounds().toFloat();
}

}  // namespace neo
