#ifndef SETTINGSWIDGET_HPP
#define SETTINGSWIDGET_HPP

#include <QWidget>

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

  private:
    Ui::SettingsWidget *ui;
};

#endif // SETTINGSWIDGET_HPP
