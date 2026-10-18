#pragma once

#include <array>
#include <cstddef>

#include <QVector2D>
#include <QVector3D>
#include <QVector4D>

using Face = std::array<std::array<size_t, 3>, 3>;
using Vertex = QVector3D;
using ScreenVertex = QVector4D;
using TextureVertex = QVector2D;
