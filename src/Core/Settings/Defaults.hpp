#pragma once

#include <QColor>

namespace Defaults
{

constexpr auto kCompanyName = "Default";
constexpr auto kApplicationName = "3D_Engine";

namespace Camera
{
constexpr float kFov = 60.F;
constexpr float kZNear = 0.1F;
constexpr float kZFar = 1000.F;
} // namespace Camera

namespace Model
{
constexpr auto kModelPath = "";
}

namespace Scene
{
constexpr QColor kLightColor = QColor(255.F, 255.F, 255.F, 255.F);
constexpr float kAmbient = 0.15F;
constexpr float kDiffuse = 1.0F;
constexpr float kSpecular = 0.15F;
} // namespace Scene

namespace Movement
{
constexpr float kMovementSpeed = 10.0F;
constexpr float kAcceleration = 15.0F;
constexpr float kMouseSensitivity = 0.002F;
constexpr float kRotationSmoothing = 25.0F;
} // namespace Movement
} // namespace Defaults