#pragma once

#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>

#include "Renderer/Mesh.hpp"
#include "Scene/Model.hpp"
#include "Scene/Scene.hpp"

class QOpenGLShaderProgram;

class SceneRenderer final : protected QOpenGLFunctions
{
    Q_DISABLE_COPY_MOVE(SceneRenderer)

  public:
    SceneRenderer();
    virtual ~SceneRenderer() = default;

    void draw(QOpenGLShaderProgram *program);

    void updateModel();

  private:
    void initModel();
    void initTextures();

    Scene scene_;
    Model model_;
    size_t index{};
};
