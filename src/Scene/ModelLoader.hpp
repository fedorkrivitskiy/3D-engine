#pragma once

#include "Scene/Model.hpp"
#include <string>

class OBJLoader
{
  public:
    OBJLoader() = default;

    void loadModel(const std::string &path);

    [[nodiscard]] const OBJModel &getModel() const noexcept;
    [[nodiscard]] OBJModel &getModel() noexcept;

  private:
    OBJModel model_;
};