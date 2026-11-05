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

        model.getMesh(i).setColorTexture(path);
        i++;
    });

    std::vector<QString> normalsFilenames = {
        "hellknight_body_n.png",
        "hellknight_head_n.png",
        "hellknight_legs_n.png",
        "hellknight_eyes_n.png",
    };

    std::ranges::for_each(normalsFilenames, [this, i = 0](const QString &filename) mutable {
        QString path =
            QString("%1/%2/%3").arg(Constants::kModelDirectoryPath, Constants::kTexturesDirectoryPath, filename);

        model.getMesh(i).setNormalsTexture(path);
        i++;
    });

    std::vector<QString> glossinessFilenames = {
        "hellknight_body_g.png",
        "hellknight_head_g.png",
        "hellknight_legs_g.png",
        "hellknight_eyes_e.png",
    };

    std::ranges::for_each(glossinessFilenames, [this, i = 0](const QString &filename) mutable {
        QString path =
            QString("%1/%2/%3").arg(Constants::kModelDirectoryPath, Constants::kTexturesDirectoryPath, filename);

        model.getMesh(i).setGlossinessTexture(path);
        i++;
    });

    std::vector<QString> specularFilenames = {
        "hellknight_body_s.png",
        "hellknight_head_s.png",
        "hellknight_legs_s.png",
        "hellknight_eyes_s.png",
    };

    std::ranges::for_each(specularFilenames, [this, i = 0](const QString &filename) mutable {
        QString path =
            QString("%1/%2/%3").arg(Constants::kModelDirectoryPath, Constants::kTexturesDirectoryPath, filename);

        model.getMesh(i).setSpecularTexture(path);
        i++;
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
            auto normals = scene.getNormals();
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

                const size_t vn0 = face[0][2];
                const size_t vn1 = face[1][2];
                const size_t vn2 = face[2][2];

                const auto &p0 = modelVertices[v0];
                const auto &p1 = modelVertices[v1];
                const auto &p2 = modelVertices[v2];

                const auto &uv0 = textureVertices[vt0];
                const auto &uv1 = textureVertices[vt1];
                const auto &uv2 = textureVertices[vt2];

                QVector3D edge1 = p1 - p0;
                QVector3D edge2 = p2 - p0;
                QVector2D deltaUV1 = uv1 - uv0;
                QVector2D deltaUV2 = uv2 - uv0;

                float r = 1.0f / (deltaUV1.x() * deltaUV2.y() - deltaUV1.y() * deltaUV2.x());
                QVector3D tangent = (edge1 * deltaUV2.y() - edge2 * deltaUV1.y()) * r;
                tangent.normalize();

                vertices.push_back({p0, uv0, normals[vn0], tangent});
                vertices.push_back({p1, uv1, normals[vn1], tangent});
                vertices.push_back({p2, uv2, normals[vn2], tangent});

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