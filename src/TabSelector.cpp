#include "TabSelector.hpp"

namespace mc
{

TabSelector::~TabSelector()
{
    std::for_each(begin(_items), end(_items), [this](auto& item) { item.button->removeListener(this); });
}

auto TabSelector::addTab(ItemSpec spec) -> void
{
    _items.push_back(spec);
    spec.button->addListener(this);
    spec.button->setRadioGroupId(_radioGroupId);
    spec.button->setClickingTogglesState(true);
}

auto TabSelector::selectFirstTab() -> void
{
    jassert(!_items.empty());
    _items[0].button->setToggleState(true, juce::NotificationType::sendNotification);
}

auto TabSelector::buttonClicked(juce::Button* button) -> void
{
    auto found = std::find_if(begin(_items), end(_items), [button](auto& item) { return item.button == button; });
    if (found == end(_items))
    {
        jassertfalse;
        return;
    }

    std::for_each(begin(_items), end(_items), [](auto& item) { item.content->setVisible(false); });
    found->content->setVisible(true);
}

auto TabSelector::setContentBounds(juce::Rectangle<int> bounds) -> void
{
    std::for_each(begin(_items), end(_items), [bounds](ItemSpec& item) { item.content->setBounds(bounds); });
}
}  // namespace mc
