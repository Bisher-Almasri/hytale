#include "Chunk.hpp"
#include "Rendering/Meshes/Cube.hpp"
#include <glm/ext/matrix_transform.hpp>
#define POS(x, cubeSize) x *cubeSize * 2

std::vector<Mesh> Chunk::getMeshes() { return meshes; }

Chunk::Chunk(int chunkSize, float cubeSize, const std::string &texturePath) {
  for (int x = 0; x < chunkSize; ++x) {
    for (int z = 0; z < chunkSize; ++z) {
      glm::mat4 transform = glm::mat4(1.0);
      transform = glm::translate(
          transform, glm::vec3(POS(x, cubeSize), 0, POS(z, cubeSize)));
      meshes.push_back(Cube(cubeSize, transform, texturePath));
    }
  }
}
