#include "Scene.hpp"

#include <QVector3D>

#include <format>
#include <string>

#include "Core/Constants.hpp"
#include "Core/Types.hpp"
#include "ModelLoader.hpp"

void Scene::init()
{
    OBJLoader modelLoader;

    std::string path = std::format("{}/Hell_Knight.obj", Constants::kModelDirectoryPath);

    modelLoader.loadModel(path);
    model_ = modelLoader.getModel();
}

void Scene::rotateModel(const QVector3D &rotation)
{
    modelRotation_ += rotation;
}

void Scene::moveModel(const QVector3D &direction)
{
    modelPosition_ += direction;
}

const std::vector<Vertex> &Scene::getVertices() const
{
    return model_.vertices;
}

const std::vector<Face> &Scene::getFaces() const
{
    return model_.faces;
}

const std::vector<Vertex> &Scene::getNormals() const
{
    return model_.normals;
}

const std::vector<TextureVertex> &Scene::getTextureVertices() const
{
    return model_.textureVertices;
}

const std::vector<size_t> &Scene::getMtls() const
{
    return model_.mtls;
}
