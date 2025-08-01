#pragma once
#include "Rendering/Meshes/Cube.hpp"
#include "Rendering/Meshes/Mesh.hpp"

#include <array>
#include <glm/glm.hpp>
#include <string>
#include <vector>

class Block
{
  public:
    Block(int cubeSize, const std::string& texturePath,
          std::array<bool, 6> vFaces = {true, true, true, true, true, true},
          glm::mat4 transform = glm::mat4(1.0f))
        : cubeSize(cubeSize), texturePath(texturePath), vFaces(vFaces), transform(transform)
    {
        renderMesh();
    }

    virtual void renderMesh()
    {
        meshes.clear();
        meshes.push_back(Cube(cubeSize, transform, texturePath, vFaces));
    }

    operator Mesh() const
    {
        if (!meshes.empty())
            return meshes[0];
        throw std::runtime_error("No mesh available");
    }

  protected:
    std::vector<Mesh> meshes;
    std::string texturePath;
    int cubeSize;
    std::array<bool, 6> vFaces;
    glm::mat4 transform;
};
