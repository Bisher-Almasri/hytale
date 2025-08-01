#include "GLTFLoader.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <stb_image.h>

GLuint LoadTexture(const tinygltf::Image& image)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    GLenum format = image.component == 4 ? GL_RGBA : GL_RGB;

    glTexImage2D(GL_TEXTURE_2D, 0, format, image.width, image.height, 0, format, GL_UNSIGNED_BYTE,
                 image.image.data());

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return textureID;
}

void ProcessNode(const tinygltf::Model& model, int nodeIndex, const glm::mat4& parentTransform,
                 std::vector<Mesh>& meshes)
{
    const auto& node = model.nodes[nodeIndex];
    glm::mat4 transform = parentTransform;

    if (!node.matrix.empty())
    {
        transform *= glm::make_mat4(reinterpret_cast<const float*>(node.matrix.data()));
    }
    else
    {
        if (!node.translation.empty())
            transform =
                glm::translate(transform, glm::vec3(node.translation[0], node.translation[1],
                                                    node.translation[2]));
        if (!node.rotation.empty())
        {
            glm::quat q(node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2]);
            transform *= glm::mat4_cast(q);
        }
    }

    if (!node.scale.empty())
    {
        glm::vec3 scaleVec(node.scale[0], node.scale[1], node.scale[2]);
        std::cout << "Scale: " << scaleVec.x << ", " << scaleVec.y << ", " << scaleVec.z
                  << std::endl;
        transform = glm::scale(transform, scaleVec);
    }
    if (node.mesh >= 0)
    {
        const auto& mesh = model.meshes[node.mesh];
        for (const auto& prim : mesh.primitives)
        {
            Mesh m;
            std::vector<float> fullVertexData;

            const auto& posAccessor = model.accessors[prim.attributes.at("POSITION")];
            const auto& posView = model.bufferViews[posAccessor.bufferView];
            const auto& posBuffer = model.buffers[posView.buffer];
            const float* pos = reinterpret_cast<const float*>(
                &posBuffer.data[posView.byteOffset + posAccessor.byteOffset]);

            const float* texcoords = nullptr;
            if (prim.attributes.count("TEXCOORD_0"))
            {
                const auto& texAccessor = model.accessors[prim.attributes.at("TEXCOORD_0")];
                const auto& texView = model.bufferViews[texAccessor.bufferView];
                const auto& texBuffer = model.buffers[texView.buffer];
                texcoords = reinterpret_cast<const float*>(
                    &texBuffer.data[texView.byteOffset + texAccessor.byteOffset]);
            }

            for (size_t i = 0; i < posAccessor.count; ++i)
            {

                fullVertexData.push_back(pos[i * 3 + 0]);
                fullVertexData.push_back(pos[i * 3 + 1]);
                fullVertexData.push_back(pos[i * 3 + 2]);

                if (texcoords)
                {
                    fullVertexData.push_back(texcoords[i * 2 + 0]);
                    fullVertexData.push_back(texcoords[i * 2 + 1]);
                }
                else
                {
                    fullVertexData.push_back(0.0f);
                    fullVertexData.push_back(0.0f);
                }
            }

            const auto& idxAccessor = model.accessors[prim.indices];
            const auto& idxView = model.bufferViews[idxAccessor.bufferView];
            const auto& idxBuffer = model.buffers[idxView.buffer];
            const unsigned short* indices = reinterpret_cast<const unsigned short*>(
                &idxBuffer.data[idxView.byteOffset + idxAccessor.byteOffset]);

            GLuint vao, vbo, ebo;
            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
            glGenBuffers(1, &ebo);

            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferData(GL_ARRAY_BUFFER, fullVertexData.size() * sizeof(float),
                         fullVertexData.data(), GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                                  (void*) (3 * sizeof(float)));
            glEnableVertexAttribArray(1);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxAccessor.count * sizeof(unsigned short),
                         indices, GL_STATIC_DRAW);

            m.vao = vao;
            m.vbo = vbo;
            m.ebo = ebo;
            m.indexCount = idxAccessor.count;
            m.transform = transform;
            m.indexType = GL_UNSIGNED_SHORT;

            m.texture = 0;
            if (prim.material >= 0)
            {
                const auto& mat = model.materials[prim.material];
                if (mat.pbrMetallicRoughness.baseColorTexture.index >= 0)
                {
                    int texIndex = mat.pbrMetallicRoughness.baseColorTexture.index;
                    int imgIndex = model.textures[texIndex].source;
                    m.texture = LoadTexture(model.images[imgIndex]);
                }
                if (mat.alphaMode == "BLEND")
                {
                    m.transparent = true;
                }
            }

            meshes.push_back(m);
        }
    }

    for (int child : node.children)
    {
        ProcessNode(model, child, transform, meshes);
    }
}

std::vector<Mesh> GltfLoader::LoadModel(const std::string& path)
{
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err, warn;

    if (!loader.LoadASCIIFromFile(&model, &err, &warn, path))
    {
        throw std::runtime_error("Failed to load glTF");
    }

    std::vector<Mesh> meshes;

    for (int sceneIndex : model.scenes[model.defaultScene].nodes)
    {
        ProcessNode(model, sceneIndex, glm::mat4(1.0f), meshes);
    }

    return meshes;
}

std::vector<Mesh> GltfLoader::LoadModel(const std::string& path, glm::vec3 startingPos)
{
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err, warn;

    if (!loader.LoadASCIIFromFile(&model, &err, &warn, path))
    {
        throw std::runtime_error("Failed to load glTF");
    }

    std::vector<Mesh> meshes;

    for (int sceneIndex : model.scenes[model.defaultScene].nodes)
    {
        ProcessNode(model, sceneIndex, glm::translate(glm::mat4(1.0f), startingPos), meshes);
    }

    return meshes;
}
