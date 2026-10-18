#pragma once

#include <string>

// #include "tiny_obj_loader.h"

#include "Model.hpp"

class ModelLoader final
{
  public:
    ModelLoader() = default;
    virtual ~ModelLoader() = default;

    OBJModel loadModel(const std::string &path);
};
