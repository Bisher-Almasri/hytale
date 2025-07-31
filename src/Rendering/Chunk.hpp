#include "Rendering/Meshes/Mesh.hpp"
#include <string>
#include <vector>
class Chunk {
public:
  Chunk(int chunkSize, float cubeSize, const std::string &texturePath);
  std::vector<Mesh> getMeshes();

private:
  std::vector<Mesh> meshes;
};
