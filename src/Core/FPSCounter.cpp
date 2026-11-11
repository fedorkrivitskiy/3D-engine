#include "FPSCounter.hpp"

#include <algorithm>
#include <numeric>

void FPSCounter::update()
{
    auto currentTime = Clock::now();

    Duration deltaDuration = currentTime - lastFrameTime_;
    deltaTime_ = deltaDuration.count();
    lastFrameTime_ = currentTime;

    frameTime_ = deltaTime_ * 1000.0F;

    frameTimes_.push_back(frameTime_);
    if (frameTimes_.size() > kSampleWindow)
    {
        frameTimes_.pop_front();
    }

    frameCount_++;
    framesSinceLastUpdate_++;
    timeSinceLastFpsUpdate_ += deltaTime_;

    if (timeSinceLastFpsUpdate_ >= kUpdateInterval)
    {
        fps_ = framesSinceLastUpdate_ / timeSinceLastFpsUpdate_;
        framesSinceLastUpdate_ = 0;
        timeSinceLastFpsUpdate_ = 0.0F;

        if (frameTimes_.size() >= 2)
        {
            float sum = std::accumulate(frameTimes_.begin(), frameTimes_.end(), 0.0F);
            averageFps_ = 1000.0F / (sum / frameTimes_.size());

            auto [minIt, maxIt] = std::minmax_element(frameTimes_.begin(), frameTimes_.end());
            minFps_ = 1000.0F / *maxIt;
            maxFps_ = 1000.0F / *minIt;
        }
    }
}

float FPSCounter::getFps() const
{
    return fps_;
}

float FPSCounter::getAverageFps() const
{
    return averageFps_;
}

float FPSCounter::getMinFps() const
{
    return minFps_;
}

float FPSCounter::getMaxFps() const
{
    return maxFps_;
}

float FPSCounter::getFrameTime() const
{
    return frameTime_;
}

float FPSCounter::getAverageFrameTime() const
{
    if (frameTimes_.empty())
    {
        return 0.0F;
    }

    return std::accumulate(frameTimes_.begin(), frameTimes_.end(), 0.0F) / frameTimes_.size();
}

float FPSCounter::getDeltaTime() const
{
    return deltaTime_;
}

int FPSCounter::getTotalFrames() const
{
    return frameCount_;
}

void FPSCounter::reset()
{
    frameCount_ = 0;
    elapsedTime_ = 0.0F;
    fps_ = 0.0F;
    deltaTime_ = 0.0F;
    frameTime_ = 0.0F;
    averageFps_ = 0.0F;
    minFps_ = 0.0F;
    maxFps_ = 0.0F;
    frameTimes_.clear();
    framesSinceLastUpdate_ = 0;
    timeSinceLastFpsUpdate_ = 0.0F;
    lastUpdateTime_ = Clock::now();
    lastFrameTime_ = Clock::now();
}