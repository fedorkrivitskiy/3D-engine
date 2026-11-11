#include "Core.hpp"

#include "Settings/SettingsModel.hpp"

Core &Core::get()
{
    static Core core{};

    return core;
}

void Core::init()
{
    settingsModel_ = std::make_unique<SettingsModel>();
}

SettingsModel *Core::getSettingsModel() const
{
    return settingsModel_.get();
}
