#include "MainComponent.hpp"

namespace neo {

TabButton::TabButton(juce::String const& name) : Button{name} {}

auto TabButton::paintButton(juce::Graphics& g, bool /*isHighlighted*/, bool /*isDown*/) -> void
{
    g.setFont(juce::Font{18.0F}.boldened());
    g.setColour(juce::Colours::white);
    g.drawText(getButtonText(), getLocalBounds().toFloat(), juce::Justification::centred);
}

}  // namespace neo

MainComponent::MainComponent()
{
    // _expand.setKeyframes(0.0F, 1.0F);

    addAndMakeVisible(_pathToggle);
    addAndMakeVisible(_transitionToggle);
    addAndMakeVisible(_widgetsToggle);
    addAndMakeVisible(_bannersToggle);
    addAndMakeVisible(_gridsToggle);
    addAndMakeVisible(_path);
    addAndMakeVisible(_transition);
    addAndMakeVisible(_widgets);
    addAndMakeVisible(_banners);
    addAndMakeVisible(_grids);

    _tabs.addTab({&_pathToggle, &_path});
    _tabs.addTab({&_transitionToggle, &_transition});
    _tabs.addTab({&_widgetsToggle, &_widgets});
    _tabs.addTab({&_bannersToggle, &_banners});
    _tabs.addTab({&_gridsToggle, &_grids});
    _tabs.onTabSelect = [this](auto const& prev, auto const& next) {
        _prev = prev.button;
        _next = next.button;

        auto const expanded  = _prev->getBounds().getUnion(_next->getBounds());
        auto const keyframes = std::vector{
            neo::Keyframe{_prev->getBounds(),  0.0},
            neo::Keyframe{          expanded, 0.25},
            neo::Keyframe{_next->getBounds(),  1.0},
        };
        _expand.setKeyframes(keyframes);
        _animation.play();
    };
    _tabs.selectFirstTab();

    setSize(600, 400);
}

auto MainComponent::paint(juce::Graphics& g) -> void
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::black);
    g.fillRoundedRectangle(_pathToggle.getBounds().toFloat(), 8.0F);
    g.fillRoundedRectangle(_transitionToggle.getBounds().toFloat(), 8.0F);
    g.fillRoundedRectangle(_widgetsToggle.getBounds().toFloat(), 8.0F);
    g.fillRoundedRectangle(_bannersToggle.getBounds().toFloat(), 8.0F);
    g.fillRoundedRectangle(_gridsToggle.getBounds().toFloat(), 8.0F);

    g.setColour(juce::Colours::green);
    g.fillRoundedRectangle(_expand.get().toFloat(), 8.0F);
}

auto MainComponent::resized() -> void
{
    auto area     = getLocalBounds();
    auto controls = area.removeFromTop(area.proportionOfHeight(0.1));

    using namespace juce;
    using Track = juce::Grid::TrackInfo;

    auto grid            = juce::Grid{};
    grid.autoFlow        = Grid::AutoFlow::row;
    grid.templateColumns = {Track{1_fr}, Track{1_fr}, Track{1_fr}, Track{1_fr}, Track{1_fr}};
    grid.templateRows    = {Track{1_fr}};
    grid.autoRows        = Track{1_fr};
    grid.autoColumns     = Track{1_fr};
    grid.rowGap          = 4_px;
    grid.columnGap       = 4_px;
    grid.items           = {
        GridItem{_pathToggle},
        GridItem{_transitionToggle},
        GridItem{_widgetsToggle},
        GridItem{_bannersToggle},
        GridItem{_gridsToggle},
    };
    grid.performLayout(controls);

    _tabs.setContentBounds(area.reduced(4));
    _expand.setKeyframes(_prev->getBounds(), _next->getBounds());
}
