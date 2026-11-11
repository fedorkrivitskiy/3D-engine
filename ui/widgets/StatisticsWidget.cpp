#include "StatisticsWidget.hpp"
#include "ui_StatisticsWidget.h"

#include <format>
#include <string>

#include "Camera/Camera.hpp"
#include "Core/FPSCounter.hpp"

namespace
{
std::string toString(const QVector3D &vec)
{
    return std::format("[{:.2F}, {:.2F}, {:.2F}]", vec.x(), vec.y(), vec.z());
}
} // namespace

StatsWidget::StatsWidget(QWidget *parent) : QWidget(parent), ui_(new Ui::StatsWidget)
{
    ui_->setupUi(this);
}

StatsWidget::~StatsWidget()
{
    delete ui_;
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
    std::string cameraStatsText = getCameraStats().toString();
    std::string rendererStatsText = getRendererStats().toString();

    ui_->rendererStatsLabel->setText(QString::fromStdString(rendererStatsText));
    ui_->cameraStatsLabel->setText(QString::fromStdString(cameraStatsText));
}

StatsWidget::CameraStats StatsWidget::getCameraStats()
{
    if (camera_ == nullptr)
    {
        return {};
    }

    return CameraStats{
        .eye = camera_->getEye(), .target = camera_->getTarget(), .up = camera_->getUp(), .scale = camera_->getScale()};
}

StatsWidget::RendererStats StatsWidget::getRendererStats()
{
    if (counter_ == nullptr)
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

std::string StatsWidget::CameraStats::toString() const
{
    return std::format("Eye: {}\n"
                       "Target: {}\n"
                       "Up: {}\n"
                       "Scale: {:.2F}",
                       ::toString(eye), ::toString(target), ::toString(up), scale);
}

std::string StatsWidget::RendererStats::toString() const
{
    return std::format("FPS: {:.0F} (Avg: {:.0F}, Min: {:.0F}, Max: {:.0F})\n"
                       "Frame Time: {:.2F}ms (Avg: {:.2F}ms)\n"
                       "Total Frames: {}",
                       fps, averageFps, minFps, maxFps, frameTime, averageFrameTime, totalFrames);
}
