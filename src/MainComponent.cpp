#include "MainComponent.hpp"

#include "TrimPathEffect.hpp"

auto addFittedText(
    juce::GlyphArrangement& glyphs,
    juce::Font const& font,
    juce::String const& text,
    juce::Rectangle<float> area,
    juce::Justification justification
)
{
    glyphs.addFittedText(
        font,
        text,
        area.getX(),
        area.getY(),
        area.getWidth(),
        area.getHeight(),
        justification,
        1
    );
}

namespace mc {
PathExamples::PathExamples()
{
    _trim.keyframes<0>(0.0F, 1.0F);
    _trim.play();
}

auto PathExamples::paint(juce::Graphics& g) -> void
{
    // CANVAS
    auto canvas = getLocalBounds().toFloat();
    g.setColour(juce::Colours::black);
    g.fillRect(canvas);

    // STAR
    auto starArea = canvas.removeFromLeft(canvas.proportionOfWidth(0.5));
    auto star     = juce::Path{};
    star.addStar(starArea.getCentre(), 24, 16.0F, 32.0F);
    star.applyTransform(star.getTransformToScaleToFit(starArea.reduced(8.0F), true));
    g.setColour(juce::Colours::red);
    g.strokePath(mc::TrimPathEffect{_trim.get<0>()}(star), juce::PathStrokeType{2.0F});

    // TEXT
    auto glyphs = juce::GlyphArrangement{};
    addFittedText(glyphs, juce::Font{54.0F}, "Submit", canvas, juce::Justification::centred);
    auto submit = juce::Path{};
    glyphs.createPath(submit);
    g.strokePath(mc::TrimPathEffect{0.0, _trim.get<0>()}(submit), juce::PathStrokeType{3.0F});
}

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

    _trim.keyframes<0>(0.0F, 1.0F);

    _duration.onValueChange = [this] {
        auto const ms = std::chrono::milliseconds{static_cast<int>(_duration.getValue())};
        _trim.duration(ms);
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

GridExamples::GridExamples()
{
    for (auto& thumbnail : _thumbnails) { addAndMakeVisible(thumbnail); }
    _transition.keyframes<0>(0.0F, 1.0F);
    _transition.onTick = [this] { layout(); };
}

auto GridExamples::layout() -> void
{
    using Traits = TransitionTraits<juce::Rectangle<int>>;

    auto const pos = static_cast<double>(_transition.get<0>());
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

    auto const isVertical = getWidth() > getHeight();
    auto const rows       = juce::Array<Track>{Track{1_fr}};
    auto const columns    = juce::Array<Track>{
        Track{1_fr},
        Track{1_fr},
        Track{1_fr},
        Track{1_fr},
        Track{1_fr},
    };

    auto proxies = std::array<juce::Component, 5>{};

    auto grid            = juce::Grid{};
    grid.autoFlow        = Grid::AutoFlow::row;
    grid.templateColumns = isVertical ? columns : rows;
    grid.templateRows    = isVertical ? rows : columns;
    grid.autoRows        = Track{1_fr};
    grid.autoColumns     = Track{1_fr};
    grid.rowGap          = 16_px;
    grid.columnGap       = 16_px;
    grid.items           = {
        GridItem{proxies[0]},
        GridItem{proxies[1]},
        GridItem{proxies[2]},
        GridItem{proxies[3]},
        GridItem{proxies[4]},
    };

    grid.performLayout(getLocalBounds());

    if (_isVertical != isVertical) {
        _isVertical = isVertical;

        _current[0] = _thumbnails[0].getBounds();
        _current[1] = _thumbnails[1].getBounds();
        _current[2] = _thumbnails[2].getBounds();
        _current[3] = _thumbnails[3].getBounds();
        _current[4] = _thumbnails[4].getBounds();

        _next[0] = proxies[0].getBounds();
        _next[1] = proxies[1].getBounds();
        _next[2] = proxies[2].getBounds();
        _next[3] = proxies[3].getBounds();
        _next[4] = proxies[4].getBounds();

        layout();
        _transition.play();

    } else {
        // _current[0] = _thumbnails[0].getBounds();
        // _current[1] = _thumbnails[1].getBounds();
        // _current[2] = _thumbnails[2].getBounds();
        // _current[3] = _thumbnails[3].getBounds();
        // _current[4] = _thumbnails[4].getBounds();

        _next[0] = proxies[0].getBounds();
        _next[1] = proxies[1].getBounds();
        _next[2] = proxies[2].getBounds();
        _next[3] = proxies[3].getBounds();
        _next[4] = proxies[4].getBounds();

        layout();
    }
}

}  // namespace mc

MainComponent::MainComponent()
{
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
    _tabs.selectFirstTab();

    setSize(600, 400);
}

auto MainComponent::paint(juce::Graphics& g) -> void
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
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
}
