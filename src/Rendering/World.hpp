#pragma once

#include "Chunk.hpp"
#include "../Helpers/PerlinNoise.hpp"
#include <glm/glm.hpp>
#include <unordered_map>
#include <string>

struct ChunkCoord {
    int x, z;
    bool operator==(const ChunkCoord& other) const {
        return x == other.x && z == other.z;
    }
};

namespace std {
    template<> struct hash<ChunkCoord> {
        size_t operator()(const ChunkCoord& coord) const {
            return hash<int>()(coord.x) ^ (hash<int>()(coord.z) << 1);
        }
    };
}

class World {
public:
    World(uint32_t seed, int chunkSize, float cubeSize, const std::string& texturePath);
    std::vector<Mesh> getMeshesNear(const glm::vec3& position, int renderDistance);
    void generateChunk(const ChunkCoord& coord);

private:
    siv::PerlinNoise perlin;
    std::unordered_map<ChunkCoord, Chunk> chunks;
    int chunkSize;
    float cubeSize;
    std::string texturePath;
    int getHeight(int globalX, int globalZ);
};