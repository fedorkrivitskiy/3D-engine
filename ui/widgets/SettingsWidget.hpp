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

  public:
    explicit SettingsWidget(QWidget *parent = nullptr);
    ~SettingsWidget();

    void setModel(SettingsModel *settingsModel = nullptr);

  private:
    Ui::SettingsWidget *ui;

    SettingsModel *model_{nullptr};
};
