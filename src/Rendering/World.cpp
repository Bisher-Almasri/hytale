#include "World.hpp"
#include "Blocks/Grass.hpp"
#include "Blocks/Dirt.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <unordered_set>

World::World(uint32_t seed, int chunkSize, float cubeSize, const std::string& texturePath)
    : perlin(seed), chunkSize(chunkSize), cubeSize(cubeSize), texturePath(texturePath) {}

int World::getHeight(int globalX, int globalZ) {
    double noise = perlin.octaveNoise2((globalX * 0.05), (globalZ * 0.05), 4);
    return static_cast<int>((noise + 1.0) * 5) - 5; 
}

void World::generateChunk(const ChunkCoord& coord) {
    std::unordered_set<glm::ivec3, Vec3Hash> blockPositions;
    int baseX = coord.x * chunkSize;
    int baseZ = coord.z * chunkSize;

    for (int lx = 0; lx < chunkSize; ++lx) {
        for (int lz = 0; lz < chunkSize; ++lz) {
            int globalX = baseX + lx;
            int globalZ = baseZ + lz;
            int height = getHeight(globalX, globalZ);
            for (int y = height - 3; y <= height; ++y) {
                blockPositions.insert(glm::ivec3(lx, y, lz));
            }
        }
    }

    std::vector<float> dirtVertices;
    std::vector<unsigned int> dirtIndices;
    size_t dirtVertexCount = 0;
    GLuint dirtTexture = Cube::LoadStaticTexture("assets/textures/dirt_block.png");

    std::vector<float> grassVertices;
    std::vector<unsigned int> grassIndices;
    size_t grassVertexCount = 0;
    GLuint grassTexture = Cube::LoadStaticTexture("assets/textures/grass_block.png");

    for (int lx = 0; lx < chunkSize; ++lx) {
        for (int lz = 0; lz < chunkSize; ++lz) {
            int globalX = baseX + lx;
            int globalZ = baseZ + lz;
            int height = getHeight(globalX, globalZ);
            for (int y = height - 3; y <= height; ++y) {
                glm::ivec3 localPos(lx, y, lz);
                std::array<bool, 6> vFaces = {
                    blockPositions.find(glm::ivec3(lx - 1, y, lz)) == blockPositions.end(),
                    blockPositions.find(glm::ivec3(lx + 1, y, lz)) == blockPositions.end(),
                    blockPositions.find(glm::ivec3(lx, y - 1, lz)) == blockPositions.end(),
                    blockPositions.find(glm::ivec3(lx, y + 1, lz)) == blockPositions.end(),
                    blockPositions.find(glm::ivec3(lx, y, lz - 1)) == blockPositions.end(),
                    blockPositions.find(glm::ivec3(lx, y, lz + 1)) == blockPositions.end()
                };
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(globalX * cubeSize * 2.0f, y * cubeSize * 2.0f, globalZ * cubeSize * 2.0f));

                Cube cube(cubeSize, transform, "", vFaces);
                auto [blockVertices, blockIndices] = cube.getTransformedData();

                if (y == height) {
                    grassVertices.insert(grassVertices.end(), blockVertices.begin(), blockVertices.end());
                    for (auto idx : blockIndices) {
                        grassIndices.push_back(static_cast<unsigned int>(grassVertexCount) + idx);
                    }
                    grassVertexCount += blockVertices.size() / 5;
                } else {
                    dirtVertices.insert(dirtVertices.end(), blockVertices.begin(), blockVertices.end());
                    for (auto idx : blockIndices) {
                        dirtIndices.push_back(static_cast<unsigned int>(dirtVertexCount) + idx);
                    }
                    dirtVertexCount += blockVertices.size() / 5;
                }
            }
        }
    }

    auto createMesh = [](const std::vector<float>& verts, const std::vector<unsigned int>& inds, GLuint tex) -> Mesh {
        Mesh m;
        glGenVertexArrays(1, &m.vao);
        glBindVertexArray(m.vao);

        glGenBuffers(1, &m.vbo);
        glBindBuffer(GL_ARRAY_BUFFER, m.vbo);
        glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);

        glGenBuffers(1, &m.ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, inds.size() * sizeof(unsigned int), inds.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        m.indexCount = inds.size();
        m.transform = glm::mat4(1.0f);
        m.transparent = false;
        m.texture = tex;
        return m;
    };

    std::vector<Mesh> meshes;
    if (!dirtIndices.empty()) {
        meshes.push_back(createMesh(dirtVertices, dirtIndices, dirtTexture));
    }
    if (!grassIndices.empty()) {
        meshes.push_back(createMesh(grassVertices, grassIndices, grassTexture));
    }
    chunks[coord] = Chunk(std::move(meshes));
}

std::vector<Mesh> World::getMeshesNear(const glm::vec3& position, int renderDistance) {
    std::vector<Mesh> allMeshes;
    int playerChunkX = static_cast<int>(position.x) / (chunkSize * static_cast<int>(cubeSize) * 2);
    int playerChunkZ = static_cast<int>(position.z) / (chunkSize * static_cast<int>(cubeSize) * 2);

    for (int dx = -renderDistance; dx <= renderDistance; ++dx) {
        for (int dz = -renderDistance; dz <= renderDistance; ++dz) {
            ChunkCoord coord = {playerChunkX + dx, playerChunkZ + dz};
            if (chunks.find(coord) == chunks.end()) {
                generateChunk(coord);
            }
            auto chunkMeshes = chunks[coord].getMeshes();
            allMeshes.insert(allMeshes.end(), chunkMeshes.begin(), chunkMeshes.end());
        }
    }
    return allMeshes;
}