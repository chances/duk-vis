#include <GLFW/glfw3.h>
#include <iostream>

 #include "gl/Triangle.hpp"
#include <GL/OOGL.hpp>

using namespace std;

void windowRefresh(GLFWwindow* window);
void init();
void render();
void handleError();

GLFWwindow *window;

GL::Context* gl;

GL::Triangle* triangle;

int main() {

  if (!glfwInit())
    exit(EXIT_FAILURE);

  // glfwWindowHint(GLFW_FSAA_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(800, 600, "DukVis", NULL, NULL);
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
  render();
  glfwSwapBuffers(window);
}

void init() {
  triangle = new GL::Triangle(gl);
  triangle->BindShaders("triangle.vs.glsl", "triangle.fs.glsl");

  GL::Program* shader = triangle->GetProgram();
  GL::Color color(100, 100, 200, 255);
  GL::Vec4 colorVec(color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f);
  shader->SetUniform(shader->GetUniform("color"), colorVec);
}

void render() {
  // glClearColor(0.2f, 0.2f, 0.5f, 0.0f);

  gl->Clear();

  triangle->Draw();
}
