#pragma once

#include <filesystem>
#include <vector>

#include "FPSCounter.hpp"

class FPSLogger
{
  public:
    explicit FPSLogger(size_t reserveFrames = kDefaultReserveFramesCount);

    void record(const FPSCounter &counter);

    [[nodiscard]] bool saveToCsv(const std::filesystem::path &path) const;

    void clear();

  private:
    struct FrameMetric
    {
        int frameNumber{};
        float elapsedTime{};
        float deltaTime{};
        float frameTime{};
        float fps{};
        float avgFps{};
        float minFps{};
        float maxFps{};
    };

    static constexpr size_t kDefaultReserveFramesCount = 10'000;

    std::vector<FrameMetric> metrics_;

    float totalElapsedTime_ = 0.0F;
};