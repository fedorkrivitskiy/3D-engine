#pragma once

#include <QVector3D>

#include <vector>

#include "Model.hpp"
#include "Types.hpp"

class Scene final
{
  public:
    Scene() = default;

    virtual ~Scene() = default;

    bool init();

    void rotateModel(const QVector3D &rotation);

    void moveModel(const QVector3D &direction);

    void update();

    std::vector<Vertex> getVertices() const;
    const std::vector<Face> &getFaces() const;
    std::vector<Vertex> getNormals() const;
    const std::vector<TextureVertex> &getTextureVertices() const;
    const std::vector<size_t> &getMtls() const;

  private:
    QVector3D modelPosition_{};
    QVector3D modelRotation_{};
    std::vector<OBJModel> models_;
    int index_{};
};