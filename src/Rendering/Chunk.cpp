#include "Chunk.hpp"

#include "Blocks/Grass.hpp"
#include "Rendering/Blocks/Dirt.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <sstream>
#include <unordered_set>

#define POS(x, cubeSize) ((x) * (cubeSize) * 2.0f)

std::vector<Mesh> Chunk::getMeshes() const
{
    return meshes;
}

Chunk::Chunk(int chunkSize, float cubeSize, const std::string& texturePath)
{
    std::unordered_set<glm::ivec3, Vec3Hash> blockPositions;

    for (int x = 0; x < chunkSize; ++x)
    {
        for (int y = -chunkSize; y <= 0; ++y)
        {
            for (int z = 0; z < chunkSize; ++z)
            {
                blockPositions.insert(glm::ivec3(x, y, z));
            }
        }
    }

    for (int x = 0; x < chunkSize; ++x)
    {
        for (int y = -chunkSize; y <= 0; ++y)
        {
            for (int z = 0; z < chunkSize; ++z)
            {
                glm::mat4 transform =
                    glm::translate(glm::mat4(1.0f),
                                   glm::vec3(POS(x, cubeSize), POS(y, cubeSize), POS(z, cubeSize)));

                std::array<bool, 6> vFaces = {
                    blockPositions.find(glm::ivec3(x - 1, y, z)) == blockPositions.end(), // left
                    blockPositions.find(glm::ivec3(x + 1, y, z)) == blockPositions.end(), // right
                    blockPositions.find(glm::ivec3(x, y - 1, z)) == blockPositions.end(), // bottom
                    blockPositions.find(glm::ivec3(x, y + 1, z)) == blockPositions.end(), // top
                    blockPositions.find(glm::ivec3(x, y, z - 1)) == blockPositions.end(), // back
                    blockPositions.find(glm::ivec3(x, y, z + 1)) == blockPositions.end()  // front
                };

                if (y == 0)
                    meshes.push_back(Grass(cubeSize, vFaces, transform));
                else
                    meshes.push_back(Dirt(cubeSize, vFaces, transform));
            }
        }
    }
}
