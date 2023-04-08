#include "MainComponent.hpp"

struct Application final : juce::JUCEApplication
{
    Application()           = default;
    ~Application() override = default;

    auto getApplicationName() -> juce::String const override { return JUCE_APPLICATION_NAME_STRING; }

    auto getApplicationVersion() -> juce::String const override
    {
        return JUCE_APPLICATION_VERSION_STRING;
    }

    auto moreThanOneInstanceAllowed() -> bool override { return true; }

    auto initialise(juce::String const& commandLine) -> void override
    {
        juce::ignoreUnused(commandLine);
        mainWindow.reset(new MainWindow(getApplicationName()));
    }

    auto shutdown() -> void override { mainWindow = nullptr; }

    auto systemRequestedQuit() -> void override { quit(); }

    auto anotherInstanceStarted(juce::String const& commandLine) -> void override
    {
        juce::ignoreUnused(commandLine);
    }

    struct MainWindow final : juce::DocumentWindow
    {
        explicit MainWindow(juce::String name)
            : DocumentWindow(
                name,
                juce::Desktop::getInstance().getDefaultLookAndFeel().findColour(
                    ResizableWindow::backgroundColourId
                ),
                DocumentWindow::allButtons
            )
        {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true);

#if JUCE_IOS || JUCE_ANDROID
            setFullScreen(true);
#else
            setResizable(true, true);
            centreWithSize(getWidth(), getHeight());
#endif

            setVisible(true);
        }

        auto closeButtonPressed() -> void override
        {
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION(Application)
