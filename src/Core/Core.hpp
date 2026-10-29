#pragma once

#include <memory>

class SettingsModel;

class Core final
{
  public:
    static Core &get();

    void init();

    SettingsModel *getSettingsModel() const;

  private:
    Core() = default;

    ~Core() = default;

  private:
    std::unique_ptr<SettingsModel> settingsModel_{nullptr};
};