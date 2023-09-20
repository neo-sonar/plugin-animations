#include "AnimatedButton.hpp"

namespace neo {
AnimatedButton::AnimatedButton(juce::String const& name) : Button{name}
{
    _font.setKeyframes(1.0F, 2.0F);
    _corner.setKeyframes(0.0F, 8.0F);
    _backgroundColor.setKeyframes(juce::Colours::white, juce::Colours::green);
    _textColor.setKeyframes(juce::Colours::black, juce::Colours::white);

    _transition.setDuration(std::chrono::milliseconds{400});
    _hoverListener.onHoverEnter = [this] { _transition.forward(); };
    _hoverListener.onHoverExit  = [this] { _transition.backward(); };
}

auto AnimatedButton::paintButton(juce::Graphics& g, bool /*isHighlighted*/, bool /*isDown*/) -> void
{
    auto const area = _layout.get();

    g.setColour(_backgroundColor.get());
    g.fillRoundedRectangle(area, _corner.get());

    g.setFont(juce::Font{16.0F}.withHorizontalScale(_font.get()));
    g.setColour(_textColor.get());
    g.drawText(getButtonText(), area, juce::Justification::centred);
}

auto AnimatedButton::resized() -> void
{
    _layout.setKeyframes(getLocalBounds().toFloat(), getLocalBounds().reduced(16, 4).toFloat());
}

}  // namespace neo
