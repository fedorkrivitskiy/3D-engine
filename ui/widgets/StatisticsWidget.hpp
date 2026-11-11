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

    Q_DISABLE_COPY_MOVE(StatsWidget)

  public:
    explicit StatsWidget(QWidget *parent = nullptr);
    ~StatsWidget() override;

    void setCamera(Camera *camera);
    void setFPSCounter(FPSCounter *counter);

    void updateStats();

  private:
    struct CameraStats
    {
        QVector3D eye{};
        QVector3D target{};
        QVector3D up{};
        float scale{};

        [[nodiscard]] std::string toString() const;
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

        [[nodiscard]] std::string toString() const;
    };

    CameraStats getCameraStats();

    RendererStats getRendererStats();

    Ui::StatsWidget *ui_{nullptr};

    Camera *camera_{nullptr};
    FPSCounter *counter_{nullptr};
};
