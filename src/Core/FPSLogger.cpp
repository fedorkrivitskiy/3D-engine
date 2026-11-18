#include <format>
#include <fstream>
#include <iomanip>

#include "FPSLogger.hpp"

FPSLogger::FPSLogger(size_t reserveFrames)
{
    metrics_.reserve(reserveFrames);
}

void FPSLogger::record(const FPSCounter &counter)
{
    if (counter.getTotalFrames() == 0)
    {
        return;
    }

    totalElapsedTime_ += counter.getDeltaTime();

    metrics_.push_back({.frameNumber = counter.getTotalFrames(),
                        .elapsedTime = totalElapsedTime_,
                        .deltaTime = counter.getDeltaTime(),
                        .frameTime = counter.getFrameTime(),
                        .fps = counter.getFps(),
                        .avgFps = counter.getAverageFps(),
                        .minFps = counter.getMinFps(),
                        .maxFps = counter.getMaxFps()});
}

bool FPSLogger::saveToCsv(const std::filesystem::path &path) const
{
    std::ofstream file(path);
    if (!file.is_open())
    {
        return false;
    }

    file << "frame,elapsed_time_s,delta_time_s,frame_time_ms,fps,avg_fps,min_fps,max_fps\n";

    file << std::fixed << std::setprecision(4);

    for (const auto &metric : metrics_)
    {
        file << std::format("{},{},{},{},{},{},{},{}\n", metric.frameNumber, metric.elapsedTime, metric.deltaTime,
                            metric.frameTime, metric.fps, metric.avgFps, metric.minFps, metric.maxFps);
    }

    return true;
}

void FPSLogger::clear()
{
    metrics_.clear();

    totalElapsedTime_ = 0.0F;
}