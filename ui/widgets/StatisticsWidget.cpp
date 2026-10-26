#include "StatisticsWidget.hpp"
#include "ui_StatisticsWidget.h"

#include <format>
#include <string>

#include "Camera.hpp"
#include "FPSCounter.hpp"

namespace
{

std::string toString(const glm::vec3 &vec)
{
    return std::format("[{:.2f}, {:.2f}, {:.2f}]", vec.x, vec.y, vec.z);
}

std::string toString(const StatsWidget::CameraStats &stats)
{
    return std::format("Eye: {}\n"
                       "Target: {}\n"
                       "Up: {}\n"
                       "Scale: {:.2f}",
                       toString(stats.eye), toString(stats.target), toString(stats.up), stats.scale);
}

std::string toString(const StatsWidget::RendererStats &stats)
{
    return std::format("FPS: {:.0f} (Avg: {:.0f}, Min: {:.0f}, Max: {:.0f})\n"
                       "Frame Time: {:.2f}ms (Avg: {:.2f}ms)\n"
                       "Total Frames: {}",
                       stats.fps, stats.averageFps, stats.minFps, stats.maxFps, stats.frameTime, stats.averageFrameTime,
                       stats.totalFrames);
}
} // namespace

StatsWidget::StatsWidget(QWidget *parent) : QWidget(parent), ui(new Ui::StatsWidget)
{
    ui->setupUi(this);
}

StatsWidget::~StatsWidget()
{
    delete ui;
}

void StatsWidget::setCamera(Camera *camera)
{
    camera_ = camera;
}

void StatsWidget::setFPSCounter(FPSCounter *counter)
{
    counter_ = counter;
}

void StatsWidget::updateStats()
{
    std::string cameraStatsText = toString(getCameraStats());
    std::string rendererStatsText = toString(getRendererStats());

    ui->rendererStatsLabel->setText(QString::fromStdString(rendererStatsText));
    ui->cameraStatsLabel->setText(QString::fromStdString(cameraStatsText));
}

StatsWidget::CameraStats StatsWidget::getCameraStats()
{
    if (!camera_)
    {
        return {};
    }

    return CameraStats{
        .eye = camera_->getEye(), .target = camera_->getTarget(), .up = camera_->getUp(), .scale = camera_->getScale()};
}

StatsWidget::RendererStats StatsWidget::getRendererStats()
{
    if (!counter_)
    {
        return {};
    }

    return RendererStats{
        .averageFps = counter_->getAverageFps(),
        .averageFrameTime = counter_->getAverageFrameTime(),
        .fps = counter_->getFps(),
        .frameTime = counter_->getFrameTime(),
        .maxFps = counter_->getMaxFps(),
        .minFps = counter_->getMinFps(),
        .totalFrames = counter_->getTotalFrames(),
    };
}
