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

    Q_DISABLE_COPY_MOVE(MainWindow)

  public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

  private slots:
    void onSettingsButtonClicked();
    void onFrameUpdate();

    void initSettingsWidget();
    void initStatisticsWidget();
    void initRenderWidget();

  private: // NOLINT
    Ui::MainWindow *ui_;

    std::unique_ptr<FPSCounter> counter_;

    QTimer *updateTimer_{nullptr};

    bool isSettingsWidgetVisible_{true};
};
