#pragma once

#include <QColor>

namespace Defaults
{
namespace Camera
{
constexpr float kFov = 60.f;
constexpr float kZNear = 0.1f;
constexpr float kZFar = 1000.f;
} // namespace Camera

namespace Model
{
constexpr auto kModelPath = "";
}

namespace Scene
{
inline const QColor kLightColor = QColor::fromRgbF(1.0f, 1.0f, 1.0f, 1.0f);
constexpr float kAmbient = 0.15f;
constexpr float kDiffuse = 1.0f;
constexpr float kSpecular = 0.15f;
} // namespace Scene
} // namespace Defaults