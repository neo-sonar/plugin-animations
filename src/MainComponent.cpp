#include "MainComponent.hpp"

struct LinearInterpolation
{
    template<typename T>
    [[nodiscard]] auto operator()(T a, T b, T t) const noexcept -> T
    {
        return std::lerp(a, b, t);
    }

    template<typename T>
    [[nodiscard]] auto operator()(juce::Point<T> a, juce::Point<T> b, T t) const noexcept
        -> juce::Point<T>
    {
        return {
            std::lerp(a.x, b.x, t),
            std::lerp(a.y, b.y, t),
        };
    }
};

struct CubicInterpolation
{
    template<typename T>
    [[nodiscard]] auto operator()(juce::Point<T> p1, juce::Point<T> p2, T t) const noexcept
        -> juce::Point<T>
    {

        auto const x0 = T{0};
        auto const y0 = T{0};
        auto const x1 = p1.x;
        auto const y1 = p1.y;
        auto const x2 = p2.x;
        auto const y2 = p2.y;
        auto const x3 = T{1};
        auto const y3 = T{1};

        auto const y = [=](auto ts) {
            return static_cast<T>(
                std::pow(1 - ts, 3) * y0 + 3 * std::pow(1 - ts, 2) * ts * y1
                + 3 * (1 - ts) * std::pow(ts, 2) * y2 + std::pow(ts, 3) * y3
            );
        };

        auto const x = [=](auto ts) {
            return static_cast<T>(
                std::pow(1 - ts, 3) * x0 + 3 * std::pow(1 - ts, 2) * ts * x1
                + 3 * (1 - ts) * std::pow(ts, 2) * x2 + std::pow(ts, 3) * x3
            );
        };

        return {x(t), y(t)};
    }
};

MainComponent::MainComponent()
{
    _duration.setRange(100.0, 2'000.0, 1.0);
    _duration.setValue(1'000.0, juce::dontSendNotification);
    _transition.addItemList({"Linear", "Ease-Out", "Ease-In-Out", "Ease-In-Out-Back"}, 1);
    _play.onClick = [this] {
        _startTime = juce::Time::getCurrentTime();
        startTimerHz(60);
    };

    addAndMakeVisible(_play);
    addAndMakeVisible(_transition);
    addAndMakeVisible(_duration);
    setSize(600, 400);
}

auto MainComponent::paint(juce::Graphics& g) -> void
{
    auto makeTransition = [](int index, float ts) {
        if (index == 1) { return CubicInterpolation{}({0.0F, 0.0F}, {1.0F, 1.0F}, ts); }
        if (index == 2) { return CubicInterpolation{}({0.0F, 0.0F}, {0.58F, 1.0F}, ts); }
        if (index == 3) { return CubicInterpolation{}({0.42F, 0.0F}, {0.58F, 1.0F}, ts); }
        if (index == 4) { return CubicInterpolation{}({0.68F, -0.2F}, {0.32F, 1.2F}, ts); }
        jassertfalse;
        return juce::Point<float>{};
    };

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

    DBG(ts);

    auto const canvas = _canvas.toFloat();
    auto const ball   = juce::Rectangle{0, 0, 40, 40}
                          .toFloat()
                          .withPosition(canvas.getTopLeft())
                          .translated(0.0F, 30.0F);

    auto const startX = canvas.getTopLeft().x + 75.0F;
    auto const endX   = canvas.getBottomRight().x - 75.0F;

    auto const transition = makeTransition(_transition.getSelectedId(), ts);
    auto const x          = juce::jmap(transition.y, startX, endX);

    g.setColour(juce::Colours::red);
    g.fillRoundedRectangle(ball.withX(x), 4.0F);
}

auto MainComponent::resized() -> void
{
    auto area        = getLocalBounds();
    auto controls    = area.removeFromTop(area.proportionOfHeight(0.1));
    auto const width = controls.proportionOfWidth(0.3333);
    _play.setBounds(controls.removeFromLeft(width).reduced(4));
    _transition.setBounds(controls.removeFromLeft(width).reduced(4));
    _duration.setBounds(controls.reduced(4));

    _canvas = area.reduced(4);
}

auto MainComponent::timerCallback() -> void { repaint(); }
