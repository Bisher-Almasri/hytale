#pragma once
#include "Mesh.hpp"
#include <array>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <stb_image.h>
#include <vector>

class Cube {
public:
  void setTransform(const glm::mat4 &t) { transform = t; }

  Cube(float size = 1.0f, glm::mat4 transform = glm::mat4(1.0f),
       const std::string &texturePath = "",
       std::array<bool, 6> faceVisibility = {true, true, true, true, true,
                                             true})
      : size(size), transform(transform), texturePath(texturePath),
        faceVisibility(faceVisibility) {
    Initialize();
  }

  operator Mesh() const { return mesh; }

private:
  void Initialize() {
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int baseIndex = 0;

    auto addFace = [&](const std::vector<float> &v,
                       const std::vector<unsigned int> &i) {
      vertices.insert(vertices.end(), v.begin(), v.end());
      for (auto idx : i)
        indices.push_back(baseIndex + idx);
      baseIndex += v.size() / 5; // 5 floats per vertex
    };

    float s = size;

    // Order: left, right, bottom, top, front, back
    if (faceVisibility[0])
      addFace(
          {-s, -s, -s, 1, 0, -s, -s, s, 1, 1, -s, s, s, 0, 1, -s, s, -s, 0, 0},
          {0, 1, 2, 2, 3, 0});
    if (faceVisibility[1])
      addFace({s, -s, -s, 0, 0, s, -s, s, 1, 0, s, s, s, 1, 1, s, s, -s, 0, 1},
              {0, 1, 2, 2, 3, 0});
    if (faceVisibility[2])
      addFace(
          {-s, -s, -s, 0, 1, s, -s, -s, 1, 1, s, -s, s, 1, 0, -s, -s, s, 0, 0},
          {0, 1, 2, 2, 3, 0});
    if (faceVisibility[3])
      addFace({-s, s, -s, 0, 0, s, s, -s, 1, 0, s, s, s, 1, 1, -s, s, s, 0, 1},
              {0, 1, 2, 2, 3, 0});
    if (faceVisibility[4])
      addFace(
          {-s, -s, -s, 0, 0, s, -s, -s, 1, 0, s, s, -s, 1, 1, -s, s, -s, 0, 1},
          {0, 1, 2, 2, 3, 0});
    if (faceVisibility[5])
      addFace({-s, -s, s, 1, 0, s, -s, s, 1, 1, s, s, s, 0, 1, -s, s, s, 0, 0},
              {0, 1, 2, 2, 3, 0});

    glGenVertexArrays(1, &mesh.vao);
    glBindVertexArray(mesh.vao);

    glGenBuffers(1, &mesh.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
                 vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &mesh.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    mesh.indexCount = indices.size();
    mesh.transform = transform;
    mesh.transparent = false;
    mesh.texture = texturePath.empty() ? 0 : LoadTexture(texturePath);
  }

  GLuint LoadTexture(const std::string &path) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    int width, height, nrChannels;
    unsigned char *data =
        stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
      std::cerr << "Failed to load texture: " << path << std::endl;
      throw std::runtime_error("Texture loading failed.");
    }

    GLenum format = (nrChannels == 3) ? GL_RGB : GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    return textureID;
  }

  float size;
  glm::mat4 transform;
  std::string texturePath;
  Mesh mesh;
  std::array<bool, 6> faceVisibility; // left, right, bottom, top, front, back
};
