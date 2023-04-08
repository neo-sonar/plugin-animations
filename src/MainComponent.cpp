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

MainComponent::MainComponent()
{
    _transition.addItemList(
        {
            "Linear",
            "Ease-Out",
            "Ease-In-Out",
            "Ease-In-Out-Back",
        },
        1
    );

    _duration.onValueChange
        = [this] { _trim.duration(mc::Milliseconds<int>{static_cast<int>(_duration.getValue())}); };
    _duration.setRange(100.0, 10000.0, 1.0);
    _duration.setValue(2000.0);

    addAndMakeVisible(_play);
    addAndMakeVisible(_loader);
    addAndMakeVisible(_transition);
    addAndMakeVisible(_duration);
    setSize(600, 400);

    _trim.trigger();
}

auto MainComponent::paint(juce::Graphics& g) -> void
{
    // WINDOW
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // CANVAS
    auto canvas = _canvas.toFloat();
    g.setColour(juce::Colours::black);
    g.fillRect(canvas);

    // STAR
    auto starArea = canvas.removeFromLeft(canvas.proportionOfWidth(0.5));
    auto star     = juce::Path{};
    star.addStar(starArea.getCentre(), 24, 16.0F, 32.0F);
    star.applyTransform(star.getTransformToScaleToFit(starArea.reduced(8.0F), true));
    g.setColour(juce::Colours::red);
    g.strokePath(mc::TrimPathEffect{_trim.get()}(star), juce::PathStrokeType{2.0F});

    // TEXT
    auto glyphs = juce::GlyphArrangement{};
    addFittedText(glyphs, juce::Font{54.0F}, "Submit", canvas, juce::Justification::centred);
    auto submit = juce::Path{};
    glyphs.createPath(submit);
    g.strokePath(mc::TrimPathEffect{0.0, _trim.get()}(submit), juce::PathStrokeType{3.0F});
}

auto MainComponent::resized() -> void
{
    auto area        = getLocalBounds();
    auto controls    = area.removeFromTop(area.proportionOfHeight(0.1));
    auto const width = controls.proportionOfWidth(0.25);
    _play.setBounds(controls.removeFromLeft(width).reduced(4));
    _loader.setBounds(controls.removeFromLeft(width).reduced(4));
    _transition.setBounds(controls.removeFromLeft(width).reduced(4));
    _duration.setBounds(controls.reduced(4));

    _canvas = area.reduced(4);
}
