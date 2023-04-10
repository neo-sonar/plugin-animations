#include "MainComponent.hpp"

namespace mc {
AnimatedButton::AnimatedButton(juce::String const& name) : Button{name}
{
    _corner.keyframes<0>(0.0F, 8.0F);
    _font.keyframes<0>(1.0F, 2.0F);
    _background.keyframes<0>(juce::Colours::white, juce::Colours::green);
    _text.keyframes<0>(juce::Colours::black, juce::Colours::white);

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
    auto const area = _margin.get<0>();

    g.setColour(_background.get<0>());
    g.fillRoundedRectangle(area, _corner.get<0>());

    g.setFont(juce::Font{16.0F}.withHorizontalScale(_font.get<0>()));
    g.setColour(_text.get<0>());
    g.drawText(getButtonText(), area, juce::Justification::centred);
}

auto AnimatedButton::resized() -> void
{
    _margin.keyframes<0>(getLocalBounds().toFloat(), getLocalBounds().reduced(16, 4).toFloat());
}

}  // namespace mc
