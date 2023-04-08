#include "MainComponent.hpp"

namespace mc {
AnimatedButton::AnimatedButton(juce::String const& name) : Button{name}
{
    _font.duration(250.0);
    _margin.duration(400.0);
    _corner.duration(400.0);
    _background.duration(600.0);
    _text.duration(600.0);
}

auto AnimatedButton::paintButton(juce::Graphics& g, bool /*isHighlighted*/, bool /*isDown*/)
    -> void
{
    auto const bounds = getLocalBounds().toFloat();
    auto const area   = bounds.reduced(bounds.proportionOfWidth(_margin.get()), 0.0F);

    g.setColour(_background.get());
    g.fillRoundedRectangle(area, _corner.get());

    g.setFont(juce::Font{16.0F}.withHorizontalScale(_font.get()));
    g.setColour(_text.get());
    g.drawText(getButtonText(), area, juce::Justification::centred);
}
}  // namespace mc
