#include "Model.hpp"

void Model::addMesh(Mesh &&mesh)
{
    meshes_.push_back(std::move(mesh));
}

Mesh &Model::getMesh(size_t index)
{
    return meshes_.at(index);
}

const Mesh &Model::getMesh(size_t index) const
{
    return meshes_.at(index);
}

void Model::draw(QOpenGLShaderProgram *program, int frameIndex)
{
    std::ranges::for_each(meshes_, [=](Mesh &mesh) { mesh.draw(program, frameIndex); });
}
