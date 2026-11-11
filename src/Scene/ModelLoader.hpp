#pragma once

#include <string>

#include "Model.hpp"

class ModelLoader final
{
  public:
    // TODO: app path to constructor
    // add getVertices(), getNormals() etc.
    ModelLoader() = default;

    static OBJModel loadModel(const std::string &path);
};
