#include "Scene.hpp"

#include <QVector3D>

#include <format>
#include <memory>
#include <string>

#include "Constants.hpp"
#include "ModelLoader.hpp"
#include "Types.hpp"

bool Scene::init()
{
    std::unique_ptr<ModelLoader> parser{new ModelLoader{}};

    models_.reserve(Constants::kFramesCount);

    for (int i = 0; i < Constants::kFramesCount; ++i)
    {
        std::string path = std::format("{}/Hell Knight{:04d}.obj", Constants::kModelDirectoryPath, i);

        models_.push_back(parser->loadModel(path));
    }

    return true;
}

void Scene::rotateModel(const QVector3D &rotate_vector)
{
    modelRotation_ += rotate_vector;
}

void Scene::moveModel(const QVector3D &move_vector)
{
    modelPosition_ += move_vector;
}

void Scene::update()
{
    index_ = (index_ + 1) % Constants::kFramesCount;
}

std::vector<Vertex> Scene::getVertices() const
{
    return models_[index_].vertices;
}

const std::vector<Face> &Scene::getFaces() const
{
    return models_[index_].faces;
}

std::vector<Vertex> Scene::getNormals() const
{
    return models_[index_].normals;
}

const std::vector<TextureVertex> &Scene::getTextureVertices() const
{
    return models_[index_].textureVertices;
}

const std::vector<size_t> &Scene::getMtls() const
{
    return models_[index_].mtls;
}
