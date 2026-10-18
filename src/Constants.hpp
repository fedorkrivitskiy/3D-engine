#pragma once

#include <numbers>

namespace Constants
{
constexpr auto kApplicationName = "3D Engine";

constexpr int kWindowWidth{1920};
constexpr int kWindowHeight{1200};
constexpr int kMaxFps{144};

constexpr float kFOV{45.0f};
constexpr float kZNear{3.0f};
constexpr float kZFar{7.0f};
constexpr float kAspect = static_cast<float>(kWindowWidth) / kWindowHeight;

constexpr float kMaxCameraPitch = 89.f;
constexpr float kMaxCameraPitchRadians = std::numbers::pi_v<float> * kMaxCameraPitch / 180.f;

constexpr float kMouseSensitivity{0.05f};
constexpr float kKeyboardSensitivity{0.1f};

constexpr float kRotationAngle = 2.0f;
constexpr float kMoveDistance = 2.0f;

constexpr float kRotationSensitivity{0.1f};
constexpr float kScaleSensitivity{0.2f};
constexpr float kMinScaleFactor{0.2f};
constexpr float kMaxScaleFactor{5.0f};
constexpr float kCameraSpeed{1.0f};

constexpr int kNumberTextures = 4;

constexpr float a = 250.f;
constexpr float ka = 0.1f;
constexpr float kd = 0.5f;
constexpr float ks = 0.3f;

constexpr auto kFontPath = "C:/Windows/Fonts/arial.ttf";
constexpr auto kModelDirectoryPath = "D:/models/Hell Knight";
constexpr auto kTexturesDirectoryPath = "Textures";

constexpr int kFramesCount = 77;
constexpr int kModelFps = 60;
} // namespace Constants
