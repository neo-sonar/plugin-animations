#include "MainComponent.hpp"

MainComponent::MainComponent()
{
    _play.onClick = [this] {
        _startTime = juce::Time::getCurrentTime();
        startTimerHz(60);
    };

    _transition.addItemList(
        {
            "Linear",
            "Ease-Out",
            "Ease-In-Out",
            "Ease-In-Out-Back",
        },
        1
    );

    _duration.onValueChange = [this] {
        // auto const ms = juce::roundToInt(_duration.getValue());
        // _play.transitionTime(ms);
    };
    _duration.setRange(100.0, 2'000.0, 1.0);
    _duration.setValue(1'000.0);

    addAndMakeVisible(_play);
    addAndMakeVisible(_loader);
    addAndMakeVisible(_transition);
    addAndMakeVisible(_duration);
    setSize(600, 400);
}

auto MainComponent::paint(juce::Graphics& g) -> void
{

    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    g.setColour(juce::Colours::black);
    g.fillRect(_canvas);

    auto const maxTimeMs = static_cast<juce::int64>(_duration.getValue());
    auto const maxTime   = juce::RelativeTime::milliseconds(maxTimeMs);

    auto const now = juce::Time::getCurrentTime();
    auto delta     = now - _startTime;

    if (delta > maxTime) {
        delta = maxTime;
        stopTimer();
    }

    auto const ts = static_cast<float>(delta.inMilliseconds())
                  / static_cast<float>(maxTime.inMilliseconds());

    auto const canvas = _canvas.toFloat();
    auto const ball   = juce::Rectangle{0, 0, 40, 40}
                          .toFloat()
                          .withPosition(canvas.getTopLeft())
                          .translated(0.0F, 30.0F);

    auto const startX = canvas.getTopLeft().x + 75.0F;
    auto const endX   = canvas.getBottomRight().x - 75.0F;

    auto const transition = mc::makeTransition(_transition.getSelectedId(), ts);
    auto const x          = juce::jmap(transition.y, startX, endX);

    g.setColour(juce::Colours::red);
    g.fillRoundedRectangle(ball.withX(x), 4.0F);
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

auto MainComponent::timerCallback() -> void { repaint(); }
