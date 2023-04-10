#include "MainComponent.hpp"

namespace mc {
AnimatedButton::AnimatedButton(juce::String const& name) : Button{name}
{
    _font.keyframes<FontIndex>(1.0F, 2.0F);
    _layout.keyframes<CornerIndex>(0.0F, 8.0F);
    _color.keyframes<BackgroundIndex>(juce::Colours::white, juce::Colours::green);
    _color.keyframes<TextIndex>(juce::Colours::black, juce::Colours::white);

    _font.duration(std::chrono::milliseconds{250});
    _layout.duration(std::chrono::milliseconds{400});
    _color.duration(std::chrono::milliseconds{600});

    _hoverListener.onHoverEnter = [this] {
        _font.forward();
        _layout.forward();
        _color.forward();
    };

    _hoverListener.onHoverExit = [this] {
        _font.backward();
        _layout.backward();
        _color.backward();
    };
}

auto AnimatedButton::paintButton(juce::Graphics& g, bool /*isHighlighted*/, bool /*isDown*/) -> void
{
    auto const area = _layout.get<MarginIndex>();

    g.setColour(_color.get<BackgroundIndex>());
    g.fillRoundedRectangle(area, _layout.get<CornerIndex>());

    g.setFont(juce::Font{16.0F}.withHorizontalScale(_font.get<FontIndex>()));
    g.setColour(_color.get<TextIndex>());
    g.drawText(getButtonText(), area, juce::Justification::centred);
}

auto AnimatedButton::resized() -> void
{
    _layout.keyframes<MarginIndex>(
        getLocalBounds().toFloat(),
        getLocalBounds().reduced(16, 4).toFloat()
    );
}

}  // namespace mc
