#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gl/Camera.hpp"
#include "gl/Triangle.hpp"
#include <GL/OOGL.hpp>

using namespace std;

void windowRefresh(GLFWwindow* window);
void init();
void render();
void handleError();

GLFWwindow *window;

GL::Context* gl;

int width = 800, height = 600;
GL::Camera* camera;
GL::Triangle* triangle;

int main() {

  if (!glfwInit())
    exit(EXIT_FAILURE);

  // glfwWindowHint(GLFW_FSAA_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(width, height, "DukVis", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  GL::Context context = GL::Context::UseExistingContext();
  gl = &context;

  glfwSetWindowRefreshCallback(window, windowRefresh);

  glfwMakeContextCurrent(window);

  init();

  while (!glfwWindowShouldClose(window)) {
    glfwMakeContextCurrent(window);

    render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(EXIT_SUCCESS);
}

void windowRefresh(GLFWwindow* window) {
  glfwGetFramebufferSize(window, &width, &height);
  camera->Resize(width, height);

  render();
  glfwSwapBuffers(window);
}

void init() {
  camera = new GL::Camera(width, height);
  // camera->MoveTo(glm::vec3(5,5,5));

  triangle = new GL::Triangle(gl);
  triangle->BindShaders("triangle.vs.glsl", "triangle.fs.glsl");
}

double oldTime = 0.0;
int rTime = 100;

void render() {
  // glClearColor(0.2f, 0.2f, 0.5f, 0.0f);

  GL::Program* shader = triangle->GetProgram();
  glUseProgram(*shader);

  double time = glfwGetTime();
  rTime += ((time - oldTime) * 127.5);
  if (rTime > 255) rTime = 0;

  glm::mat4 model = glm::mat4(1.0f);
  // rotation.RotateX(GL::Rad(static_cast<float>(rTime)));
  // model.RotateY(GL::Rad(static_cast<float>((rTime / 255.0) * 360.0)));

  glUniformMatrix4fv(shader->GetUniform("model"), 1, false, glm::value_ptr(camera->Project(model)));

  GL::Color color(rTime, 100, 200, 255);
  GL::Vec4 colorVec(color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f);
  shader->SetUniform(shader->GetUniform("color"), colorVec);

  oldTime = time;

  gl->Clear();

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  triangle->Draw();

  glDisable(GL_DEPTH_TEST);

  // Setup 2D projection
  glm::mat4 projection = glm::ortho(0, width, height, 0);

  glUniformMatrix4fv(shader->GetUniform("model"), 1, false, glm::value_ptr(projection));

  color = GL::Color(255, 0, 0, 255);
  colorVec = GL::Vec4(color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f);
  shader->SetUniform(shader->GetUniform("color"), colorVec);

  triangle->Draw();
}
