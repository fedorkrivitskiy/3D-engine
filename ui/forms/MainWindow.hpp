#pragma once

#include <QMainWindow>

#include <QSet>
#include <QVector3D>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QTimer;

class FPSCounter;

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

  private slots:
    void onSettingsButtonClicked();
    void onFrameUpdate();

  private:
    void initSettingsWidget();
    void initStatisticsWidget();
    void initRenderWidget();

  private:
    Ui::MainWindow *ui;

    std::unique_ptr<FPSCounter> counter_;

    QTimer *frameUpdateTimer_{nullptr};

    bool isSettingsWidgetVisible{true};
};
