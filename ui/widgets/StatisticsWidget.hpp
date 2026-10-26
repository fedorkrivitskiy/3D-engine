#pragma once

#include <QWidget>

#include <glm/vec3.hpp>

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
        glm::vec3 eye{};
        glm::vec3 target{};
        glm::vec3 up{};
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
