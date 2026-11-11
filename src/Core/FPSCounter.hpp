#pragma once

#include <chrono>
#include <deque>

class FPSCounter final
{
  public:
    FPSCounter() = default;

    void update();

    void reset();

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
    float elapsedTime_{0.0F};
    float fps_{0.0F};
    float deltaTime_{0.0F};

    float frameTime_{0.0F};
    std::deque<float> frameTimes_;
    float averageFps_{0.0F};
    float minFps_{0.0F};
    float maxFps_{0.0F};

    static constexpr float kUpdateInterval{0.5F};
    static constexpr int kSampleWindow{60};

    float timeSinceLastFpsUpdate_{0.0F};
    int framesSinceLastUpdate_{0};
};