#include "ModelLoader.hpp"

#include <iostream>
#include <vector>

#include "Core/Types.hpp"
#include "Scene/Model.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
// #define TINYOBJLOADER_USE_MAPBOX_EARCUT
#include "tiny_obj_loader.h"

const OBJModel &OBJLoader::getModel() const noexcept
{
    return model_;
}

OBJModel &OBJLoader::getModel() noexcept
{
    return model_;
}

void OBJLoader::loadModel(const std::string &path)
{
    model_ = OBJModel{};

    tinyobj::ObjReaderConfig readerConfig{};
    tinyobj::ObjReader reader{};

    if (!reader.ParseFromFile(path, readerConfig))
    {
        if (!reader.Error().empty())
        {
            std::cerr << "TinyObjReader Error: " << reader.Error() << '\n';
        }
        return;
    }

    if (!reader.Warning().empty())
    {
        std::cout << "TinyObjReader Warning: " << reader.Warning() << '\n';
    }

    const auto &attrib = reader.GetAttrib();
    const auto &shapes = reader.GetShapes();

    model_.vertices.reserve(attrib.vertices.size() / 3);
    for (size_t i = 0; i < attrib.vertices.size(); i += 3)
    {
        model_.vertices.emplace_back(attrib.vertices[i + 0], attrib.vertices[i + 1], attrib.vertices[i + 2]);
    }

    model_.normals.reserve(attrib.normals.size() / 3);
    for (size_t i = 0; i < attrib.normals.size(); i += 3)
    {
        model_.normals.emplace_back(attrib.normals[i + 0], attrib.normals[i + 1], attrib.normals[i + 2]);
    }

    model_.textureVertices.reserve(attrib.texcoords.size() / 2);
    for (size_t i = 0; i < attrib.texcoords.size(); i += 2)
    {
        model_.textureVertices.emplace_back(attrib.texcoords[i + 0], attrib.texcoords[i + 1]);
    }

    using VertexIndices = std::array<size_t, 3>;

    int currentMaterialId = -2;
    int currentMtlFaceCount = 0;

    for (const auto &shape : shapes)
    {
        size_t indexOffset = 0;

        for (size_t faceVertexIndex = 0; faceVertexIndex < shape.mesh.num_face_vertices.size(); ++faceVertexIndex)
        {
            const auto faceVertex = static_cast<size_t>(shape.mesh.num_face_vertices[faceVertexIndex]);
            const int matId = shape.mesh.material_ids[faceVertexIndex];

            if (matId != currentMaterialId)
            {
                if (currentMaterialId != -2)
                {
                    model_.mtls.push_back(currentMtlFaceCount);
                }
                currentMaterialId = matId;
                currentMtlFaceCount = 0;
            }

            std::vector<VertexIndices> faceVertices;
            faceVertices.reserve(faceVertex);

            for (size_t vertexIndex = 0; vertexIndex < faceVertex; ++vertexIndex)
            {
                const tinyobj::index_t idx = shape.mesh.indices[indexOffset + vertexIndex];

                const size_t vIdx = idx.vertex_index >= 0 ? static_cast<size_t>(idx.vertex_index) : 0;
                const size_t vtIdx = idx.texcoord_index >= 0 ? static_cast<size_t>(idx.texcoord_index) : 0;
                const size_t vnIdx = idx.normal_index >= 0 ? static_cast<size_t>(idx.normal_index) : 0;

                faceVertices.push_back({vIdx, vtIdx, vnIdx});
            }

            for (size_t i = 1; i + 1 < faceVertices.size(); ++i)
            {
                model_.faces.push_back(Face{faceVertices[0], faceVertices[i], faceVertices[i + 1]});
                currentMtlFaceCount++;
            }

            indexOffset += faceVertex;
        }
    }

    if (currentMaterialId != -2 && currentMtlFaceCount > 0)
    {
        model_.mtls.push_back(currentMtlFaceCount);
    }
}