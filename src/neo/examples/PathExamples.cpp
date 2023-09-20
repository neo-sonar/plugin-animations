#include "PathExamples.hpp"

#include "neo/graphics/TrimPathEffect.hpp"

namespace neo {

static auto addFittedText(
    juce::GlyphArrangement& glyphs,
    juce::Font const& font,
    juce::String const& text,
    juce::Rectangle<float> area,
    juce::Justification justification
)
{
    glyphs.addFittedText(font, text, area.getX(), area.getY(), area.getWidth(), area.getHeight(), justification, 1);
}

PathExamples::PathExamples()
{
    _trim.setKeyframes(0.0F, 1.0F);
    _animation.play();
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
    g.strokePath(neo::TrimPathEffect{_trim.get()}(star), juce::PathStrokeType{2.0F});

    // TEXT
    auto glyphs = juce::GlyphArrangement{};
    addFittedText(glyphs, juce::Font{54.0F}, "Submit", canvas, juce::Justification::centred);
    auto submit = juce::Path{};
    glyphs.createPath(submit);
    g.strokePath(neo::TrimPathEffect{0.0, _trim.get()}(submit), juce::PathStrokeType{3.0F});
}

}  // namespace neo
