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
class QElapsedTimer;

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
    void keyReleaseEvent(QKeyEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

  private slots:
    void onSettingsButtonClicked();
    void onFrameUpdate();

  private:
    void initSettingsWidget();
    void initStatisticsWidget();
    void initRenderWidget();

    void updateCameraMovement(float deltaTime);
    void updateCameraRotation(float deltaTime);

  private:
    Ui::MainWindow *ui;

    std::unique_ptr<FPSCounter> counter_;
    std::unique_ptr<Camera> camera_;

    QTimer *frameUpdateTimer_{nullptr};

    bool isSettingsWidgetVisible{true};

    QSet<int> pressedKeys_;
    QElapsedTimer *deltaTimer_;
    QVector3D currentVelocity_{0.0f, 0.0f, 0.0f};

    bool isMouseLookActive_{false};
    QPointF lastMousePos_;
    QVector3D pendingRotation_{0.0f, 0.0f, 0.0f};
};
