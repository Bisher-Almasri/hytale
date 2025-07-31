#pragma once
// clang-format off
#include <glad/glad.h>
// clang-format on
#include "GLFW/glfw3.h"
#include "Rendering/Renderer.hpp"
#include <Helpers/Camera.hpp>
#include <Helpers/Shader.hpp>
#include <Rendering/Meshes/GLTFLoader.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

struct MeshData {
  GLuint VAO, VBO, EBO;
  GLsizei indexCount;
};
class Game {
public:
  Game(GLuint width, GLuint height);
  ~Game();
  void Init(GLFWwindow *window);
  void Update(float deltaTime);
  void Render();
  void handleInput(GLFWwindow *window, float deltaTime);
  Camera *getCamera();

private:
  GLuint width, height;
  Shader *shader;
  Camera *camera;
  Renderer *renderer;
  std::vector<Mesh> modelMeshes;
  std::vector<Mesh> chunkMeshes;
  float speed = 25.5f;
  bool locked = true;
  bool wf = false;
};
