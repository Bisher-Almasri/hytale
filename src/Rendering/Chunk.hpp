#pragma once
#include "Rendering/Meshes/Mesh.hpp"

#include <string>
#include <unordered_set>
#include <vector>

struct Vec3Hash
{
    std::size_t operator()(const glm::ivec3& v) const
    {
        return ((std::hash<int>()(v.x) ^ (std::hash<int>()(v.y) << 1)) >> 1) ^
               (std::hash<int>()(v.z) << 1);
    }
};

class Chunk
{
  public:
    Chunk(int chunkSize, float cubeSize, const std::string& texturePath);
    std::vector<Mesh> getMeshes() const;
    Mesh createChunkMesh(const std::unordered_set<glm::ivec3, Vec3Hash>& blockPositions,
                         int chunkSize, float cubeSize, const std::string& texturePath);

  private:
    std::vector<Mesh> meshes;
};
