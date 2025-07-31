#include "Game.hpp"
#include <GLFW/glfw3.h>
#include <Rendering/Chunk.hpp>
#include <Rendering/Meshes/Cube.hpp>
#include <Rendering/Renderer.hpp>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>
#include <iostream>
#include <tiny_gltf.h>

Game::Game(GLuint width, GLuint height)
    : width(width), height(height), shader(nullptr), camera(nullptr) {}

void Game::Init(GLFWwindow *window) {
  shader = new Shader("assets/shaders/vert.glsl", "assets/shaders/frag.glsl");
  camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f), speed);
  modelMeshes = GltfLoader::LoadModel("assets/models/Base.gltf");
  renderer = new Renderer(shader, camera, width, height);

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  io.Fonts->AddFontFromFileTTF("assets/Inter_18pt-Regular.ttf", 18.0f);
  io.FontGlobalScale = 1.0f;
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init("#version 330");
  ImGuiStyle &style = ImGui::GetStyle();
  ImVec4 *colors = style.Colors;

  colors[ImGuiCol_Text] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
  colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
  colors[ImGuiCol_ChildBg] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
  colors[ImGuiCol_FrameBg] = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
  colors[ImGuiCol_FrameBgHovered] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
  colors[ImGuiCol_FrameBgActive] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
  colors[ImGuiCol_TitleBg] = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
  colors[ImGuiCol_TitleBgActive] = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
  colors[ImGuiCol_Button] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
  colors[ImGuiCol_ButtonHovered] = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
  colors[ImGuiCol_ButtonActive] = ImVec4(0.40f, 0.40f, 0.40f, 1.00f);
  colors[ImGuiCol_Border] = ImVec4(0.25f, 0.25f, 0.25f, 1.00f);
  colors[ImGuiCol_SliderGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
  colors[ImGuiCol_SliderGrabActive] = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
  colors[ImGuiCol_ResizeGrip] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
  colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
  colors[ImGuiCol_ResizeGripActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
  colors[ImGuiCol_Tab] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);

  style.WindowRounding = 5.0f;
  style.FrameRounding = 4.0f;
  style.GrabRounding = 4.0f;
  style.ScrollbarRounding = 4.0f;
  style.FramePadding = ImVec2(6, 4);
  style.ItemSpacing = ImVec2(10, 8);
  style.IndentSpacing = 20.0f;

  float cubeSize = 1.0f;
  int chunkSize = 16;
  Chunk chunk(chunkSize, cubeSize, "assets/textures/grass_block_side.png");
  chunkMeshes = chunk.getMeshes();
  std::cout << chunk.getMeshes().size();
}

void Game::Update(float deltaTime) {}

void Game::handleInput(GLFWwindow *window, float deltaTime) {
  float lastB, lastC = 0.0f;
  float now = glfwGetTime();

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera->ProcessKeyboard(FORWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera->ProcessKeyboard(BACKWARD, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera->ProcessKeyboard(LEFT, deltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera->ProcessKeyboard(RIGHT, deltaTime);

  if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && now - lastC > 0.3) {
    locked = !locked;
    glfwSetInputMode(window, GLFW_CURSOR,
                     locked ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    lastC = now;
  }

  if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS && now - lastB > 0.3) {
    wf = !wf;
    glPolygonMode(GL_FRONT_AND_BACK, wf ? GL_LINE : GL_FILL);
    lastB = now;
  }
}

void Game::Render() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();
  // renderer->Render(modelMeshes);

  for (const auto &chunkMesh : chunkMeshes) {
    renderer->Render(chunkMesh);
  }

  ImGui::Begin("camera");
  ImGui::SliderFloat("speed", &speed, 0, 50);
  ImGui::Checkbox("cam lock", &locked);
  ImGui::Checkbox("wf", &wf);
  ImGui::End();

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  camera->MovementSpeed = speed;
}

Camera *Game::getCamera() { return camera; }

Game::~Game() {
  delete shader;
  delete camera;
}
