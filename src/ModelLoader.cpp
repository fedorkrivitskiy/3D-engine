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

#include "Model.hpp"
#include "Types.hpp"

OBJModel ModelLoader::loadModel(const std::string &file_path)
{
    std::ifstream file(file_path);
    if (!file.is_open())
    {
        return {};
    }

    OBJModel model{};

    std::string line;
    int index_ = 0;

    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v")
        {
            float x{}, y{}, z{};
            iss >> x >> y >> z;

            model.vertices.push_back(QVector3D{x, y, z});
        }
        else if (type == "f")
        {
            index_++;
            std::vector<std::array<size_t, 3>> face_vertices;
            std::string vertex_data;

            while (iss >> vertex_data)
            {
                std::vector<std::string> parts;
                size_t start = 0;
                size_t end = vertex_data.find('/');

                while (true)
                {
                    parts.push_back(vertex_data.substr(start, end - start));
                    if (end == std::string::npos)
                        break;

                    start = end + 1;
                    end = vertex_data.find('/', start);
                }

                uint32_t v = 0, vt = 0, vn = 0;

                if (!parts[0].empty())
                    v = std::stoi(parts[0]);
                if (parts.size() > 1 && !parts[1].empty())
                    vt = std::stoi(parts[1]);
                if (parts.size() > 2 && !parts[2].empty())
                    vn = std::stoi(parts[2]);

                v = (v != 0) ? v - 1 : -1;
                vt = (vt != 0) ? vt - 1 : -1;
                vn = (vn != 0) ? vn - 1 : -1;

                face_vertices.push_back({v, vt, vn});
            }

            size_t num_verts = face_vertices.size();
            if (num_verts < 3)
                continue;

            for (size_t i = 1; i < num_verts - 1; ++i)
            {
                model.faces.push_back(Face{face_vertices[0], face_vertices[i], face_vertices[i + 1]});
            }
        }
        else if (type == "vn")
        {
            float x{}, y{}, z{};
            iss >> x >> y >> z;

            model.normals.push_back(QVector3D{x, y, z});
        }
        else if (type == "vt")
        {
            float x{}, y{};
            iss >> x >> y;

            model.textureVertices.push_back(QVector2D{x, y});
        }
        else if (type == "usemtl")
        {
            model.mtls.push_back(index_);
            index_ = 0;
        }
    }

    auto &mtls = model.mtls;
    const auto &faces = model.faces;

    long long sum = std::accumulate(mtls.begin(), mtls.end(), 0LL);
    std::shift_left(mtls.begin(), mtls.end(), 1);
    mtls.back() = faces.size() - sum;

    return model;
}