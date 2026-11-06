#pragma once

#include <QWidget>

class SettingsModel;

namespace Ui
{
class SettingsWidget;
}

class SettingsWidget : public QWidget
{
    Q_OBJECT

    Q_DISABLE_COPY_MOVE(SettingsWidget);

  public:
    explicit SettingsWidget(QWidget *parent = nullptr);
    ~SettingsWidget() override;

    void setModel(SettingsModel *settingsModel = nullptr);

  private:
    Ui::SettingsWidget *ui_;

    SettingsModel *model_{nullptr};
};
