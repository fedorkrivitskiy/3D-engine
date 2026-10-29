#pragma once

#include <QVector3D>
#include <QWidget>

class FPSCounter;
class Camera;

namespace Ui
{
class StatsWidget;
}

class StatsWidget : public QWidget
{
    Q_OBJECT

  public:
    struct CameraStats
    {
        QVector3D eye{};
        QVector3D target{};
        QVector3D up{};
        float scale{};
    };

    struct RendererStats
    {
        float averageFps{};
        float averageFrameTime{};
        float fps{};
        float frameTime{};
        float maxFps{};
        float minFps{};
        int totalFrames{};
    };

    explicit StatsWidget(QWidget *parent = nullptr);
    ~StatsWidget();

    void setCamera(Camera *camera);
    void setFPSCounter(FPSCounter *counter);

    void updateStats();

  private:
    CameraStats getCameraStats();
    RendererStats getRendererStats();

  private:
    Ui::StatsWidget *ui;

    Camera *camera_{nullptr};
    FPSCounter *counter_{nullptr};
};
