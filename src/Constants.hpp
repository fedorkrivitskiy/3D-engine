#pragma once

#include <numbers>

namespace Constants
{
constexpr float kFOV{45.0f};
constexpr float kZNear{3.0f};
constexpr float kZFar{7.0f};

constexpr float kMaxCameraPitch = 89.f;
constexpr float kMaxCameraPitchRadians = std::numbers::pi_v<float> * kMaxCameraPitch / 180.f;

constexpr int kNumberTextures = 4;

constexpr float a = 250.f;
constexpr float ka = 0.1f;
constexpr float kd = 0.5f;
constexpr float ks = 0.3f;

constexpr auto kModelDirectoryPath = "D:/models/Hell Knight";
constexpr auto kTexturesDirectoryPath = "Textures";

constexpr int kFramesCount = 77;
} // namespace Constants
