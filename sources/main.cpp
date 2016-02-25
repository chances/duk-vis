#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "gl/Camera.hpp"
#include "gl/Element.hpp"
#include <GL/OOGL.hpp>

using namespace std;

void windowRefresh(GLFWwindow* window);
void init();
void render();

GLFWwindow *window;

GL::Context* gl;

int width = 800, height = 600;
GL::Camera* camera;

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

GL::VertexArray* vao = 0;
GL::VertexBuffer* vbo;
GL::Program* shader;
GL::Uniform modelUniform;
GL::Uniform colorUniform;

void init() {
  camera = new GL::Camera(width, height);
  camera->MoveTo(glm::vec3(5,3,2));

  shader = Element::bindShaders("triangle.vs.glsl", "triangle.fs.glsl");
  modelUniform = shader->GetUniform("model");
  colorUniform = shader->GetUniform("color");

  glUniformMatrix4fv(modelUniform, 1, false, glm::value_ptr(glm::mat4(1.0f)));
  GL::Color color = GL::Color(255, 0, 0, 50);
  GL::Vec4 colorVec = GL::Vec4(color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f);
  shader->SetUniform(shader->GetUniform("color"), colorVec);

  float vertices[] {
    -0.5f,  0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f
  };
  Element::vertices verts(vertices);
  verts.numVerts = ARRAY_SIZE(vertices);
  Element::create(vao, vbo, shader, &verts);
}

double oldTime = 0.0;
double rTime = 0.0;

void render() {
  // glClearColor(0.2f, 0.2f, 0.5f, 0.0f);

  double time = glfwGetTime();
  rTime += ((time - oldTime) * 255);
  if (rTime > 255) rTime = 0;

  glm::mat4 model = glm::mat4(1.0f);
  // rotation.RotateX(GL::Rad(static_cast<float>(rTime)));
  model = glm::rotate(model, glm::radians((float)((rTime / 255) * 360.0)), glm::vec3(0,1,0));
  // model = glm::rotateY(model, (float)((rTime / 255.0) * 360.0));

  glUniformMatrix4fv(modelUniform, 1, false, glm::value_ptr(camera->Project(model)));

  // GL::Color color(rTime, 100, 200, 255);
  // GL::Vec4 colorVec(color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f);
  // shader->SetUniform(colorUniform, colorVec);

  oldTime = time;

  gl->Clear();

  // glEnable(GL_DEPTH_TEST);
  // glDepthFunc(GL_LESS);

  Element::draw(vao);

  // glDisable(GL_DEPTH_TEST);

  // Setup 2D projection
  glm::mat4 projection = glm::ortho(0, width, height, 0);

  glUniformMatrix4fv(shader->GetUniform("model"), 1, false, glm::value_ptr(projection));

  GL::Color color = GL::Color(255, 20, 20, 50);
  GL::Vec4 colorVec = GL::Vec4(color.R / 255.0f, color.G / 255.0f, color.B / 255.0f, color.A / 255.0f);
  shader->SetUniform(shader->GetUniform("color"), colorVec);

  Element::draw(vao);
}
