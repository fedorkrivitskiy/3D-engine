#pragma once

#include <chrono>
#include <deque>

class FPSCounter final
{
  public:
    FPSCounter() = default;

    void update();

    void reset();

    void setSampleWindow(int samples);

    [[nodiscard]] float getFps() const;

    [[nodiscard]] float getAverageFps() const;

    [[nodiscard]] float getMinFps() const;

    [[nodiscard]] float getMaxFps() const;

    [[nodiscard]] float getFrameTime() const;

    [[nodiscard]] float getAverageFrameTime() const;

    [[nodiscard]] float getDeltaTime() const;

    [[nodiscard]] int getTotalFrames() const;

  private:
    using Clock = std::chrono::steady_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    using Duration = std::chrono::duration<float>;

  private:
    TimePoint lastUpdateTime_{Clock::now()};
    TimePoint lastFrameTime_{Clock::now()};
    int frameCount_{0};
    float elapsedTime_{0.0f};
    float fps_{0.0f};
    float deltaTime_{0.0f};

    float frameTime_{0.0f};
    std::deque<float> frameTimes_;
    int sampleWindow_{60};
    float averageFps_{0.0f};
    float minFps_{0.0f};
    float maxFps_{0.0f};

    float updateInterval_{0.5f};
    float timeSinceLastFpsUpdate_{0.0f};
    int framesSinceLastUpdate_{0};
};