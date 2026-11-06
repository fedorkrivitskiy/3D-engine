#pragma once

#include "Core/Types.hpp"
#include "Renderer/Mesh.hpp"

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

    void addMesh(Mesh &&mesh);

    Mesh &getMesh(size_t index);

    [[nodiscard]] const Mesh &getMesh(size_t index) const;

    void draw(QOpenGLShaderProgram *program, int frameIndex);

  private:
    std::vector<Mesh> meshes_;
};
