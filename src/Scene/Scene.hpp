#pragma once

#include <QVector3D>

#include <vector>

#include "Core/Types.hpp"
#include "Model.hpp"

class Scene final
{
  public:
    Scene() = default;

    bool init();

    void rotateModel(const QVector3D &rotation);

    void moveModel(const QVector3D &direction);

    void update();

    [[nodiscard]] std::vector<Vertex> getVertices() const;

    [[nodiscard]] const std::vector<Face> &getFaces() const;

    [[nodiscard]] std::vector<Vertex> getNormals() const;

    [[nodiscard]] const std::vector<TextureVertex> &getTextureVertices() const;

    [[nodiscard]] const std::vector<size_t> &getMtls() const;

  private:
    QVector3D modelPosition_{};
    QVector3D modelRotation_{};
    std::vector<OBJModel> models_;
    size_t index_{};
};