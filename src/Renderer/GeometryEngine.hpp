#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>

#include "Renderer/Mesh.hpp"
#include "Scene/Model.hpp"
#include "Scene/Scene.hpp"

class QOpenGLShaderProgram;

class GeometryEngine final : protected QOpenGLFunctions
{
    Q_DISABLE_COPY_MOVE(GeometryEngine)

  public:
    GeometryEngine();
    virtual ~GeometryEngine() = default;

    void drawModel(QOpenGLShaderProgram *program);

    void updateModel();

  private:
    void initModel();
    void initTextures();

  private:
    Scene scene_;
    Model model_;
    int index_{};
};
