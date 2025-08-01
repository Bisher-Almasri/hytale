#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"

#include <Helpers/Shader.hpp>
#include <vector>

struct Mesh
{
    GLuint vao, vbo, ebo, texture;
    size_t indexCount;
    GLenum indexType = GL_UNSIGNED_INT;
    glm::mat4 transform;
    bool transparent = false;
};

// class Model {
// public:
//   void addMesh(const Mesh &mesh);
//   void render(Shader *shader);
//
// private:
//   std::vector<Mesh> meshes;
// };
