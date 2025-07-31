#pragma once
#include "Mesh.hpp"
#include "tiny_gltf.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class GltfLoader {
public:
  static std::vector<Mesh> LoadModel(const std::string &path);
};
