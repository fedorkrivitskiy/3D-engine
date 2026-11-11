#include "ModelLoader.hpp"

#include <QVector3D>

#include <algorithm>
#include <array>
#include <climits>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iosfwd>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include "Core/Types.hpp"
#include "Scene/Model.hpp"

OBJModel ModelLoader::loadModel(const std::string &path)
{
    std::ifstream file(path);
    if (!file.is_open())
    {
        return {};
    }

    OBJModel model{};

    std::string line;
    int index = 0;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v")
        {
            float vertexX{}, vertexY{}, vertexZ{};
            iss >> vertexX >> vertexY >> vertexZ;

            model.vertices.emplace_back(vertexX, vertexY, vertexZ);
        }
        else if (type == "f")
        {
            index++;
            std::vector<std::array<size_t, 3>> faceVertices;
            std::string vertexData;

            while (iss >> vertexData)
            {
                std::vector<std::string> parts;
                size_t start = 0;
                size_t end = vertexData.find('/');

                while (true)
                {
                    parts.push_back(vertexData.substr(start, end - start));
                    if (end == std::string::npos) {
                        break;
                    }

                    start = end + 1;
                    end = vertexData.find('/', start);
                }

                uint32_t vertexIndex = 0, textureVertexIndex = 0, normalVertexIndex = 0;

                if (!parts[0].empty()) {
                    vertexIndex = std::stoi(parts[0]);
                }
                if (parts.size() > 1 && !parts[1].empty()) {
                    textureVertexIndex = std::stoi(parts[1]);
                }
                if (parts.size() > 2 && !parts[2].empty()) {
                    normalVertexIndex = std::stoi(parts[2]);
                }

                vertexIndex = (vertexIndex != 0) ? vertexIndex - 1 : -1;
                textureVertexIndex = (textureVertexIndex != 0) ? textureVertexIndex - 1 : -1;
                normalVertexIndex = (normalVertexIndex != 0) ? normalVertexIndex - 1 : -1;

                faceVertices.push_back({vertexIndex, textureVertexIndex, normalVertexIndex});
            }

            size_t numVerts = faceVertices.size();
            if (numVerts < 3) {
                continue;
            }

            for (size_t i = 1; i < numVerts - 1; ++i)
            {
                model.faces.push_back(Face{faceVertices[0], faceVertices[i], faceVertices[i + 1]});
            }
        }
        else if (type == "vn")
        {
            float vertexX{}, vertexY{}, vertexZ{};
            iss >> vertexX >> vertexY >> vertexZ;

            model.normals.emplace_back(vertexX, vertexY, vertexZ);
        }
        else if (type == "vt")
        {
            float vertexX{}, vertexY{};
            iss >> vertexX >> vertexY;

            model.textureVertices.emplace_back(vertexX, vertexY);
        }
        else if (type == "usemtl")
        {
            model.mtls.push_back(index);
            index = 0;
        }
    }

    auto &mtls = model.mtls;
    const auto &faces = model.faces;

    size_t sum = std::accumulate(mtls.begin(), mtls.end(), size_t{});
    std::shift_left(mtls.begin(), mtls.end(), 1);
    mtls.back() = faces.size() - sum;

    return model;
}