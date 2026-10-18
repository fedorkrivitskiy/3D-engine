#include "geometryengine.h"

#include <QOpenGLShaderProgram>
#include <QString>

#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

#include "Constants.hpp"
#include "Mesh.hpp"
#include "Types.hpp"

GeometryEngine::GeometryEngine()
{
    scene.init();
    initializeOpenGLFunctions();

    initModel();
    initTextures();
}

void GeometryEngine::initTextures()
{
    std::vector<QString> filenames = {
        "hellknight_body.png",
        "hellknight_head.png",
        "hellknight_legs.png",
        "hellknight_eyes.png",
    };

    std::ranges::for_each(filenames, [this, i = 0](const QString &filename) mutable {
        QString path =
            QString("%1/%2/%3").arg(Constants::kModelDirectoryPath, Constants::kTexturesDirectoryPath, filename);

        model.getMesh(i++).setTexture(path);
    });
}

void GeometryEngine::initModel()
{
    const auto &modelMtls = scene.getMtls();

    size_t offset = 0;

    for (size_t i = 0; i < modelMtls.size(); ++i)
    {
        std::vector<std::vector<VertexData>> meshVertices{};
        meshVertices.reserve(Constants::kFramesCount);

        std::vector<Index> meshIndicies{};

        size_t currentSize = modelMtls[i];

        for (size_t j = 0; j < Constants::kFramesCount; ++j)
        {
            auto modelVertices = scene.getVertices();
            auto textureVertices = scene.getTextureVertices();
            auto modelFaces = scene.getFaces();
            scene.update();

            std::vector<VertexData> vertices;
            vertices.reserve(currentSize * 3);

            std::vector<Index> indices;
            indices.reserve(currentSize * 3);

            GLuint currentIndex = 0;

            for (size_t j = 0; j < currentSize; ++j)
            {
                const Face &face = modelFaces[offset + j];

                const size_t v0 = face[0][0];
                const size_t v1 = face[1][0];
                const size_t v2 = face[2][0];

                const size_t vt0 = face[0][1];
                const size_t vt1 = face[1][1];
                const size_t vt2 = face[2][1];

                vertices.push_back({modelVertices[v0], textureVertices[vt0]});
                vertices.push_back({modelVertices[v1], textureVertices[vt1]});
                vertices.push_back({modelVertices[v2], textureVertices[vt2]});

                indices.push_back(currentIndex++);
                indices.push_back(currentIndex++);
                indices.push_back(currentIndex++);
            }

            meshVertices.push_back(std::move(vertices));
            meshIndicies = std::move(indices);
        }

        Mesh mesh{};
        mesh.setVertices(meshVertices);
        mesh.setIndices(meshIndicies);

        model.addMesh(std::move(mesh));

        offset += currentSize;
    }
}

void GeometryEngine::drawModel(QOpenGLShaderProgram *program)
{
    model.draw(program, index);
}

void GeometryEngine::updateModel()
{
    index = (index + 1) % Constants::kFramesCount;
}