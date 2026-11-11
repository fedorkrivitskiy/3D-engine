#include "GeometryEngine.hpp"

#include <QOpenGLShaderProgram>
#include <QString>

#include <algorithm>
#include <cstddef>
#include <utility>
#include <vector>

#include "Core/Constants.hpp"
#include "Core/Types.hpp"
#include "Mesh.hpp"

GeometryEngine::GeometryEngine()
{
    scene_.init();
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

    std::ranges::for_each(filenames, [this, index = 0](const QString &filename) mutable {
        QString path =
            QString("%1/%2/%3").arg(Constants::kModelDirectoryPath, Constants::kTexturesDirectoryPath, filename);

        model_.getMesh(index).setColorTexture(path);
        index++;
    });

    std::vector<QString> normalsFilenames = {
        "hellknight_body_n.png",
        "hellknight_head_n.png",
        "hellknight_legs_n.png",
        "hellknight_eyes_n.png",
    };

    std::ranges::for_each(normalsFilenames, [this, index = 0](const QString &filename) mutable {
        QString path =
            QString("%1/%2/%3").arg(Constants::kModelDirectoryPath, Constants::kTexturesDirectoryPath, filename);

        model_.getMesh(index).setNormalsTexture(path);
        index++;
    });

    std::vector<QString> glossinessFilenames = {
        "hellknight_body_g.png",
        "hellknight_head_g.png",
        "hellknight_legs_g.png",
        "hellknight_eyes_e.png",
    };

    std::ranges::for_each(glossinessFilenames, [this, index = 0](const QString &filename) mutable {
        QString path =
            QString("%1/%2/%3").arg(Constants::kModelDirectoryPath, Constants::kTexturesDirectoryPath, filename);

        model_.getMesh(index).setGlossinessTexture(path);
        index++;
    });

    std::vector<QString> specularFilenames = {
        "hellknight_body_s.png",
        "hellknight_head_s.png",
        "hellknight_legs_s.png",
        "hellknight_eyes_s.png",
    };

    std::ranges::for_each(specularFilenames, [this, index = 0](const QString &filename) mutable {
        QString path =
            QString("%1/%2/%3").arg(Constants::kModelDirectoryPath, Constants::kTexturesDirectoryPath, filename);

        model_.getMesh(index).setSpecularTexture(path);
        index++;
    });
}

void GeometryEngine::initModel()
{
    const auto &modelMtls = scene_.getMtls();

    size_t offset = 0;

    for (size_t currentSize : modelMtls)
    {
        std::vector<std::vector<VertexData>> meshVertices{};
        meshVertices.reserve(Constants::kFramesCount);

        std::vector<Index> meshIndicies{};

        for (size_t j = 0; j < Constants::kFramesCount; ++j)
        {
            auto modelVertices = scene_.getVertices();
            auto normals = scene_.getNormals();
            auto textureVertices = scene_.getTextureVertices();
            auto modelFaces = scene_.getFaces();
            scene_.update();

            std::vector<VertexData> vertices;
            vertices.reserve(currentSize * 3);

            std::vector<Index> indices;
            indices.reserve(currentSize * 3);

            GLuint currentIndex = 0;

            for (size_t j = 0; j < currentSize; ++j)
            {
                const Face &face = modelFaces[offset + j];

                const size_t vertex0Index = face[0][0];
                const size_t vertex1Index = face[1][0];
                const size_t vertex2Index = face[2][0];

                const size_t vertexTexture0Index = face[0][1];
                const size_t vertexTexture1Index = face[1][1];
                const size_t vertexTexture2Index = face[2][1];

                const size_t vertexNormal0Index = face[0][2];
                const size_t vertexNormal1Index = face[1][2];
                const size_t vertexNormal2Index = face[2][2];

                const auto &vertex0 = modelVertices[vertex0Index];
                const auto &vertex1 = modelVertices[vertex1Index];
                const auto &vertex2 = modelVertices[vertex2Index];

                const auto &textureVertex0 = textureVertices[vertexTexture0Index];
                const auto &textureVertex1 = textureVertices[vertexTexture1Index];
                const auto &textureVertex2 = textureVertices[vertexTexture2Index];

                QVector3D edge1 = vertex1 - vertex0;
                QVector3D edge2 = vertex2 - vertex0;
                QVector2D deltaUV1 = textureVertex1 - textureVertex0;
                QVector2D deltaUV2 = textureVertex2 - textureVertex0;

                float reciprocalDeterminant = 1.0F / ((deltaUV1.x() * deltaUV2.y()) - (deltaUV1.y() * deltaUV2.x()));
                QVector3D tangent = ((edge1 * deltaUV2.y()) - (edge2 * deltaUV1.y())) * reciprocalDeterminant;
                tangent.normalize();

                vertices.push_back({vertex0, textureVertex0, normals[vertexNormal0Index], tangent});
                vertices.push_back({vertex1, textureVertex1, normals[vertexNormal1Index], tangent});
                vertices.push_back({vertex2, textureVertex2, normals[vertexNormal2Index], tangent});

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

        model_.addMesh(std::move(mesh));

        offset += currentSize;
    }
}

void GeometryEngine::drawModel(QOpenGLShaderProgram *program)
{
    model_.draw(program, index_);
}

void GeometryEngine::updateModel()
{
    index_ = (index_ + 1) % Constants::kFramesCount;
}