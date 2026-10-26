#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QTimer;

class SettingsModel;
class FPSCounter;
class Camera;

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

  protected:
    void keyPressEvent(QKeyEvent *event) override;

  private:
    void onSettingsButtonClicked();

  private:
    Ui::MainWindow *ui;

    SettingsModel *settingsModel_{nullptr};

    std::unique_ptr<FPSCounter> counter_;
    std::unique_ptr<Camera> camera_;

    QTimer *timer_{nullptr};

    bool isSettingsWidgetVisible{true};
};
