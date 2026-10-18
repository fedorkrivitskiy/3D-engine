#pragma once

#include "Mesh.hpp"
#include "Types.hpp"

#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>

struct OBJModel
{
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    std::vector<Vertex> normals;
    std::vector<TextureVertex> textureVertices;
    std::vector<size_t> mtls;
};

struct MeshData
{
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    std::vector<Vertex> normals;
    std::vector<TextureVertex> textureVertices;
};

struct Model
{
  public:
    Model() = default;

    void addMesh(Mesh &&mesh)
    {
        meshes_.push_back(std::forward<Mesh>(mesh));
    }

    Mesh &getMesh(size_t index)
    {
        return meshes_[index];
    }

    const Mesh &getMesh(size_t index) const
    {
        return meshes_[index];
    }

    void draw(QOpenGLShaderProgram *program, int frameIndex)
    {
        std::ranges::for_each(meshes_, [=](Mesh &mesh) { mesh.draw(program, frameIndex); });
    }

  private:
    std::vector<Mesh> meshes_{};
};
