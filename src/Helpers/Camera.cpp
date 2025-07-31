#include "Camera.hpp"

Camera::Camera(glm::vec3 position, float speed, glm::vec3 up, float yaw,
               float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(speed),
      MouseSensitivity(SENSITIVITY) {
  Position = position;
  WorldUp = up;
  Yaw = yaw;
  Pitch = pitch;
  updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY,
               float upZ, float yaw, float pitch)
    : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED),
      MouseSensitivity(SENSITIVITY) {
  Position = glm::vec3(posX, posY, posZ);
  WorldUp = glm::vec3(upX, upY, upZ);
  Yaw = yaw;
  Pitch = pitch;
  updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
  return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(const Camera_Movement direction,
                             const float deltaTime) {
  const float velocity = MovementSpeed * deltaTime;
  if (direction == FORWARD)
    Position += Front * velocity;
  if (direction == BACKWARD)
    Position -= Front * velocity;
  if (direction == LEFT)
    Position -= Right * velocity;
  if (direction == RIGHT)
    Position += Right * velocity;
  if (direction == UPWARD)
    Position += Up * velocity;
  if (direction == DOWNWARD)
    Position -= Up * velocity;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset,
                                  const GLboolean constrainPitch) {
  xOffset *= MouseSensitivity;
  yOffset *= MouseSensitivity;

  Yaw += xOffset;
  Pitch += yOffset;

  if (constrainPitch) {
    if (Pitch > 89.0f)
      Pitch = 89.0f;
    if (Pitch < -89.0f)
      Pitch = -89.0f;
  }

  updateCameraVectors();
}

void Camera::updateCameraVectors() {
  glm::vec3 front;
  front.x = static_cast<float>(cos(glm::radians(Yaw))) *
            static_cast<float>(cos(glm::radians(Pitch)));
  front.y = static_cast<float>(sin(glm::radians(Pitch)));
  front.z = static_cast<float>(sin(glm::radians(Yaw))) *
            static_cast<float>(cos(glm::radians(Pitch)));
  Front = glm::normalize(front);
  Right = glm::normalize(glm::cross(Front, WorldUp));
  Up = glm::normalize(glm::cross(Right, Front));
}
