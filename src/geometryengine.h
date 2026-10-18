#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>

#include "Mesh.hpp"
#include "Model.hpp"
#include "Scene.hpp"

class QOpenGLShaderProgram;

class GeometryEngine final : protected QOpenGLFunctions
{
  public:
    GeometryEngine();
    virtual ~GeometryEngine() = default;

    void drawModel(QOpenGLShaderProgram *program);

    void updateModel();

  private:
    void initModel();
    void initTextures();

  private:
    Scene scene{};
    Model model{};
    int index = 0;
};
