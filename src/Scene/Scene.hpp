#pragma once

#include <QVector3D>

#include <vector>

#include "Core/Types.hpp"
#include "Model.hpp"

class Scene final
{
  public:
    Scene() = default;

    void init();

    void drawModel(QOpenGLShaderProgram *program);

    void updateModel();

    void rotateModel(const QVector3D &rotation);

    void moveModel(const QVector3D &direction);

    void update();

    [[nodiscard]] const std::vector<Vertex> &getVertices() const;

    [[nodiscard]] const std::vector<Face> &getFaces() const;

    [[nodiscard]] const std::vector<Vertex> &getNormals() const;

    [[nodiscard]] const std::vector<TextureVertex> &getTextureVertices() const;

    [[nodiscard]] const std::vector<size_t> &getMtls() const;

  private:
    QVector3D modelPosition_;
    QVector3D modelRotation_;
    OBJModel model_;
};