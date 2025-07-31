#pragma once
#include "Meshes/Mesh.hpp"
#include <Helpers/Camera.hpp>
#include <Helpers/Shader.hpp>
#include <vector>

class Renderer {
public:
  Renderer(Shader *shader, Camera *camera, GLuint width, GLuint height);
  void Render(const std::vector<Mesh> &modelMeshes);
  void Render(const Mesh &modelMesh);

private:
  Shader *shader;
  Camera *camera;
  GLuint width, height;
};
