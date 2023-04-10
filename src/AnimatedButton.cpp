#include "MainComponent.hpp"

namespace mc {
AnimatedButton::AnimatedButton(juce::String const& name) : Button{name}
{
    _margin.keyframes({}, {});
    _corner.keyframes(0.0F, 8.0F);
    _font.keyframes(1.0F, 2.0F);
    _background.keyframes(juce::Colours::white, juce::Colours::green);
    _text.keyframes(juce::Colours::black, juce::Colours::white);

    _font.duration(std::chrono::milliseconds{250});
    _margin.duration(std::chrono::milliseconds{400});
    _corner.duration(std::chrono::milliseconds{400});
    _background.duration(std::chrono::milliseconds{600});
    _text.duration(std::chrono::milliseconds{600});

    _hoverListener.onHoverEnter = [this] {
        _margin.forward();
        _corner.forward();
        _font.forward();
        _background.forward();
        _text.forward();
    };

    _hoverListener.onHoverExit = [this] {
        _margin.backward();
        _corner.backward();
        _font.backward();
        _background.backward();
        _text.backward();
    };
}

auto AnimatedButton::paintButton(juce::Graphics& g, bool /*isHighlighted*/, bool /*isDown*/) -> void
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
    _margin.keyframes(getLocalBounds().toFloat(), getLocalBounds().reduced(16, 4).toFloat());
}

}  // namespace mc
