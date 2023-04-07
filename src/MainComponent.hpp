#pragma once

#include <span>

#include <juce_gui_extra/juce_gui_extra.h>

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

inline constexpr auto makeTransition = [](int index, float ts) {
    if (index == 1) { return CubicInterpolation{}({0.0F, 0.0F}, {1.0F, 1.0F}, ts); }
    if (index == 2) { return CubicInterpolation{}({0.0F, 0.0F}, {0.58F, 1.0F}, ts); }
    if (index == 3) { return CubicInterpolation{}({0.42F, 0.0F}, {0.58F, 1.0F}, ts); }
    if (index == 4) { return CubicInterpolation{}({0.68F, -0.2F}, {0.32F, 1.2F}, ts); }
    return juce::Point<float>{};
};

struct AnimationTimer final : juce::Timer
{
    AnimationTimer()           = default;
    ~AnimationTimer() override = default;

    auto duration(double ms) -> void { _duration = ms; }

    [[nodiscard]] auto position() const -> double { return _position; }

    auto restart(bool forward) -> void
    {
        _forward   = forward;
        _startTime = juce::Time::getCurrentTime();
        // _position  = 0.0;
        startTimerHz(30);
    }

    std::function<void()> onTick;

private:
    auto timerCallback() -> void override
    {
        auto const maxTime = juce::RelativeTime::milliseconds(juce::roundToInt(_duration));
        auto const now     = juce::Time::getCurrentTime();
        auto delta         = now - _startTime;

        if (delta > maxTime) {
            delta = maxTime;
            stopTimer();
        }

        _position = static_cast<double>(delta.inMilliseconds())
                  / static_cast<double>(maxTime.inMilliseconds());

        if (not _forward) { _position = 1.0 - _position; }
        if (onTick) { onTick(); }
    }

    bool _forward{true};
    double _position{0.0};
    double _duration{1000};
    juce::Time _startTime{};
};

struct AnimationTrigger
{
    explicit AnimationTrigger(AnimationTimer& timer) : _timer{timer} {}

    virtual ~AnimationTrigger() = default;

protected:
    auto triggerUp() -> void { _timer.restart(true); }

    auto triggerDown() -> void { _timer.restart(false); }

    AnimationTimer& _timer;
};

struct AnimationTriggers
{
    struct Hover final
        : AnimationTrigger
        , juce::MouseListener
    {
        explicit Hover(AnimationTimer& timer, juce::Component* component)
            : AnimationTrigger{timer}
            , _component{component}
        {
            component->addMouseListener(this, true);
        }

        ~Hover() override { _component->removeMouseListener(this); }

    private:
        auto mouseEnter(juce::MouseEvent const& /*event*/) -> void override { triggerUp(); }

        auto mouseExit(juce::MouseEvent const& /*event*/) -> void override
        {
            triggerDown();
        }

        juce::Component* _component;
    };
};

template<typename T>
struct AnimatedPropertyInterpolator
{
    static constexpr auto interpolate(T a, T b, double t) -> T
    {
        return std::lerp(a, b, static_cast<T>(t));
    }
};

template<>
struct AnimatedPropertyInterpolator<juce::Colour>
{
    static auto interpolate(juce::Colour a, juce::Colour b, double t) -> juce::Colour
    {
        return a.interpolatedWith(b, static_cast<float>(t));
    }
};

template<typename T>
struct AnimatedProperty
{
    AnimatedProperty(juce::Component* parent, T const& a, T const& b)
        : _parent{parent}
        , _trigger{std::make_unique<AnimationTriggers::Hover>(_timer, _parent)}
        , _keyframes{a, b}
    {
        _timer.onTick = [this] { _parent->repaint(); };
    }

    auto duration(double ms) { _timer.duration(ms); }

    [[nodiscard]] auto get() const -> T
    {
        return AnimatedPropertyInterpolator<T>::interpolate(
            _keyframes.front(),
            _keyframes.back(),
            _timer.position()
        );
    }

private:
    juce::Component* _parent;

    AnimationTimer _timer;
    std::unique_ptr<AnimationTrigger> _trigger;

    std::array<T, 2> _keyframes;
    double _position{0};
};

using AnimatedColor = AnimatedProperty<juce::Colour>;

struct AnimatedButton final : juce::Button
{
    explicit AnimatedButton(juce::String const& name);
    ~AnimatedButton() override = default;

    auto paintButton(juce::Graphics& g, bool isHighlighted, bool isDown) -> void override;

private:
    AnimatedProperty<float> _margin{this, 0.0F, 0.125F};
    AnimatedProperty<float> _corner{this, 0.0F, 8.0F};
    AnimatedProperty<float> _font{this, 1.0F, 2.0F};
    AnimatedColor _background{this, juce::Colours::white, juce::Colours::green};
    AnimatedColor _text{this, juce::Colours::black, juce::Colours::white};
};

struct MainComponent final
    : juce::Component
    , juce::Timer
{
    MainComponent();
    ~MainComponent() override = default;

    auto paint(juce::Graphics& g) -> void override;
    auto resized() -> void override;

    auto timerCallback() -> void override;

private:
    AnimatedButton _play{"Play"};
    juce::ComboBox _transition{"Transition"};
    juce::Slider _duration{juce::Slider::LinearHorizontal, juce::Slider::TextBoxRight};

    juce::Rectangle<int> _canvas{};
    juce::Time _startTime{};
};
