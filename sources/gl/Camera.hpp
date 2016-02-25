#pragma once

#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

namespace GL {
  class Camera {
  public:
    Camera(int width, int height);
    Camera(float fov, float aspectRatio, float near, float far);

    glm::vec3 GetPosition();
    void MoveTo(glm::vec3 position);
    void LookAt(glm::vec3 lookAt);
    void Resize(int width, int height);

    glm::mat4 Project(glm::mat4 model);
  private:
    int width;
    int height;

    glm::vec3 position = glm::vec3(4,3,3);
    glm::vec3 lookAt = glm::vec3(0,0,0);
    glm::vec3 up = glm::vec3(0,1,0);

    glm::mat4 projection;
    glm::mat4 view;
  };
}
