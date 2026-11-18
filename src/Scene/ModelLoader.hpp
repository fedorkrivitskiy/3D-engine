#pragma once

#include <array>
#include <cstddef>
#include <iosfwd>
#include <string>
#include <string_view>

#include "Model.hpp"

class ModelLoader final
{
  public:
    ModelLoader() = default;

    OBJModel loadModel(const std::string &path);

    [[nodiscard]] const OBJModel &getModel() const noexcept;

    [[nodiscard]] OBJModel &getModel() noexcept;

  private:
    void parseVertex(std::istringstream &iss);
    void parseNormal(std::istringstream &iss);
    void parseTextureCoord(std::istringstream &iss);
    void parseFace(std::istringstream &iss, int &faceCounter);

    void handleMaterialUse(int &faceCounter);
    void finalizeMaterials();

    OBJModel model_{};
};