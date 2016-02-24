#include "Camera.hpp"

namespace GL {
  Camera::Camera(int width, int height) : Camera(45.0f, (float)width / (float)height, 0.1f, 100.0f) {
    this->width = width;
    this->height = height;
  }

  Camera::Camera(float fov, float aspectRatio, float near, float far) {
    projection = glm::perspective(glm::radians(fov), aspectRatio, near, far);
    view = glm::lookAt(position, lookAt, up);
  }

  glm::vec3 Camera::GetPosition() {
    return position;
  }

  void Camera::MoveTo(glm::vec3 position) {
    this->position = position;
    view = glm::lookAt(position, lookAt, up);
  }

  void Camera::LookAt(glm::vec3 lookAt) {
    this->lookAt = lookAt;
    view = glm::lookAt(position, lookAt, up);
  }

  void Camera::Resize(int width, int height) {
    this->width = width;
    this->height = height;
    projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);
  }

  glm::mat4 Camera::Project(glm::mat4 model) {
    return projection * view * model;
  }
}
