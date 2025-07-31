#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum Camera_Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT,
  UPWARD,
  DOWNWARD,
};

constexpr float YAW = -90.0f;
constexpr float PITCH = 0.0f;
constexpr float SPEED = 2.5f;
constexpr float SENSITIVITY = 0.1f;

class Camera {
public:
  glm::vec3 Position{};
  glm::vec3 Front;
  glm::vec3 Up{};
  glm::vec3 Right{};
  glm::vec3 WorldUp{};

  float Yaw;
  float Pitch;

  float MovementSpeed;
  float MouseSensitivity;

  explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
                  float speed = SPEED,
                  glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW,
                  float pitch = PITCH);

  Camera(float posX, float posY, float posZ, float upX, float upY, float upZ,
         float yaw, float pitch);

  [[nodiscard]] glm::mat4 GetViewMatrix() const;

  void ProcessKeyboard(Camera_Movement direction, float deltaTime);

  void ProcessMouseMovement(float xOffset, float yOffset,
                            GLboolean constrainPitch = true);

private:
  void updateCameraVectors();
};
