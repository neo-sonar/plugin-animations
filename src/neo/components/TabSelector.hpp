#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

namespace neo {

struct TabSelectorItemSpec
{
    juce::Button* button;
    juce::Component* content;
};

struct TabSelector final : juce::Button::Listener
{
    using ItemSpec = TabSelectorItemSpec;

    TabSelector() = default;
    ~TabSelector() override;

    auto addTab(ItemSpec spec) -> void;
    auto selectFirstTab() -> void;
    auto setContentBounds(juce::Rectangle<int> bounds) -> void;

    std::function<void(ItemSpec const&, ItemSpec const&)> onTabSelect;

private:
    auto buttonClicked(juce::Button* button) -> void override;

    int _radioGroupId{juce::Random::getSystemRandom().nextInt()};
    std::vector<ItemSpec> _items;
    std::optional<ItemSpec> _selected;
};

}  // namespace neo
