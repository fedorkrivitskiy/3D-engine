#pragma once

#include <memory>

class SettingsModel;

class Core final
{
  public:
    static Core &get();

    void init();

    [[nodiscard]] SettingsModel *getSettingsModel() const;

  private:
    Core() = default;

    std::unique_ptr<SettingsModel> settingsModel_{nullptr};
};