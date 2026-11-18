#include "ModelLoader.hpp"

#include <algorithm>
#include <charconv>
#include <cstdint>
#include <fstream>
#include <numeric>
#include <sstream>

#include "Core/Types.hpp"
#include "Scene/Model.hpp"

namespace
{
size_t parseIndex(std::string_view token)
{
    if (token.empty())
    {
        return static_cast<size_t>(-1);
    }

    uint32_t val = 0;
    auto [ptr, ec] = std::from_chars(token.data(), token.data() + token.size(), val); // NOLINT
    if (ec == std::errc{} && val > 0)
    {
        return static_cast<size_t>(val - 1);
    }

    return static_cast<size_t>(-1);
}

using VertexIndices = std::array<size_t, 3>;

VertexIndices parseFaceVertex(std::string_view token)
{
    const size_t firstSlash = token.find('/');
    if (firstSlash == std::string_view::npos)
    {
        return {parseIndex(token), static_cast<size_t>(-1), static_cast<size_t>(-1)};
    }

    const std::string_view vStr = token.substr(0, firstSlash);

    const size_t secondSlash = token.find('/', firstSlash + 1);
    if (secondSlash == std::string_view::npos)
    {
        const std::string_view vtStr = token.substr(firstSlash + 1);
        return {parseIndex(vStr), parseIndex(vtStr), static_cast<size_t>(-1)};
    }

    const std::string_view vtStr = token.substr(firstSlash + 1, secondSlash - firstSlash - 1);
    const std::string_view vnStr = token.substr(secondSlash + 1);

    return {parseIndex(vStr), parseIndex(vtStr), parseIndex(vnStr)};
}
} // namespace

const OBJModel &ModelLoader::getModel() const noexcept
{
    return model_;
}

OBJModel &ModelLoader::getModel() noexcept
{
    return model_;
}

void ModelLoader::parseVertex(std::istringstream &iss)
{
    float vertexX{};
    float vertexY{};
    float vertexZ{};

    if (iss >> vertexX >> vertexY >> vertexZ)
    {
        model_.vertices.emplace_back(vertexX, vertexY, vertexZ);
    }
}

void ModelLoader::parseNormal(std::istringstream &iss)
{
    float normalX{};
    float normalY{};
    float normalZ{};

    if (iss >> normalX >> normalY >> normalZ)
    {
        model_.normals.emplace_back(normalX, normalY, normalZ);
    }
}

void ModelLoader::parseTextureCoord(std::istringstream &iss)
{
    float textureU{};
    float textureV{};

    if (iss >> textureU >> textureV)
    {
        model_.textureVertices.emplace_back(textureU, textureV);
    }
}

void ModelLoader::parseFace(std::istringstream &iss, int &faceCounter)
{
    faceCounter++;

    std::vector<VertexIndices> faceVertices;
    std::string token;

    while (iss >> token)
    {
        faceVertices.push_back(parseFaceVertex(token));
    }

    const size_t numVerts = faceVertices.size();
    if (numVerts < 3)
    {
        return;
    }

    for (size_t i = 1; i < numVerts - 1; ++i)
    {
        model_.faces.push_back(Face{faceVertices[0], faceVertices[i], faceVertices[i + 1]});
    }
}

void ModelLoader::handleMaterialUse(int &faceCounter)
{
    model_.mtls.push_back(faceCounter);
    faceCounter = 0;
}

void ModelLoader::finalizeMaterials()
{
    auto &mtls = model_.mtls;
    if (mtls.empty())
    {
        return;
    }

    size_t sum = std::accumulate(mtls.begin(), mtls.end(), size_t{});
    std::shift_left(mtls.begin(), mtls.end(), 1); // NOLINT
    mtls.back() = model_.faces.size() - sum;
}

OBJModel ModelLoader::loadModel(const std::string &path)
{
    model_ = OBJModel{};

    std::ifstream file(path);
    if (!file.is_open())
    {
        return {};
    }

    std::string line;
    int currentMtlFaceCount = 0;

    while (std::getline(file, line))
    {
        if (line.empty() || line[0] == '#')
        {
            continue;
        }

        std::istringstream iss(line);
        std::string type;
        iss >> type;

        if (type == "v")
        {
            parseVertex(iss);
        }
        else if (type == "f")
        {
            parseFace(iss, currentMtlFaceCount);
        }
        else if (type == "vn")
        {
            parseNormal(iss);
        }
        else if (type == "vt")
        {
            parseTextureCoord(iss);
        }
        else if (type == "usemtl")
        {
            handleMaterialUse(currentMtlFaceCount);
        }
    }

    finalizeMaterials();

    return model_;
}