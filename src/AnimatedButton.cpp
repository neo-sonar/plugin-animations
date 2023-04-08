#include "MainComponent.hpp"

namespace mc {
AnimatedButton::AnimatedButton(juce::String const& name) : Button{name}
{
    _margin.keyframes({}, {});
    _corner.keyframes(0.0F, 8.0F);
    _font.keyframes(1.0F, 2.0F);
    _background.keyframes(juce::Colours::white, juce::Colours::green);
    _text.keyframes(juce::Colours::black, juce::Colours::white);

    _font.duration(Milliseconds<int>{250});
    _margin.duration(Milliseconds<int>{400});
    _corner.duration(Milliseconds<int>{400});
    _background.duration(Milliseconds<int>{600});
    _text.duration(Milliseconds<int>{600});
}

auto AnimatedButton::paintButton(juce::Graphics& g, bool /*isHighlighted*/, bool /*isDown*/)
    -> void
{
    auto const area = _margin.get();

    g.setColour(_background.get());
    g.fillRoundedRectangle(area, _corner.get());

    g.setFont(juce::Font{16.0F}.withHorizontalScale(_font.get()));
    g.setColour(_text.get());
    g.drawText(getButtonText(), area, juce::Justification::centred);
}

auto AnimatedButton::resized() -> void
{
    _margin.keyframes(
        getLocalBounds().toFloat(),
        getLocalBounds().reduced(16, 4).toFloat()
    );
}

}  // namespace mc
