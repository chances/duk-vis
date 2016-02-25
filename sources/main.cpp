#include <GLFW/glfw3.h>
#include <iostream>
#include <map>

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

int width = 800, height = 600;
GL::Camera* camera;
glm::mat4 projection;

typedef pair<const char*, Element::object*> object;
map<const char*, Element::object*> objects;

Element::object* triangle = NULL;
Element::object* overlay = NULL;
GL::Color overlayColor = GL::Color(90, 150, 90, floor(255 * 0.65));

int main() {

  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(width, height, "DukVis", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

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
  projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

  if (overlay != NULL && overlay->shader != NULL) {
    glfwMakeContextCurrent(window);
    glUseProgram(*overlay->shader);
    glUniformMatrix4fv(overlay->uniforms.at("model"), 1, false, glm::value_ptr(
      glm::scale(glm::mat4(1.0f), glm::vec3((float)width, (float)height, 1.0))
    ));
    glUniformMatrix4fv(overlay->uniforms.at("projection"), 1, false, glm::value_ptr(projection));
  }

  render();
  glfwSwapBuffers(window);
}

void init() {
  camera = new GL::Camera(width, height);
  camera->MoveTo(glm::vec3(5,3,2));

  projection = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);

  float verticesTriangle[] {
    -0.5f,  0.5f, 0.0f,
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f
  };
  Element::vertices verts(verticesTriangle);
  verts.numVerts = ARRAY_SIZE(verticesTriangle);
  triangle = Element::create("triangle", &verts);
  triangle->bindUniform("model");
  triangle->bindUniform("color");

  float verticesOverlay[] {
    // Pos      // Tex
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f
  };
  Element::vertices vertsO(verticesOverlay);
  vertsO.numVerts = ARRAY_SIZE(verticesOverlay);
  overlay = new Element::object("overlay");
  overlay = Element::create(&vertsO, overlay, 4, true, false);
  overlay->vao->BindAttribute(overlay->shader->GetAttribute("vertex"),
    *(overlay->vbos[0]), GL::Type::Float, 4, 4 * sizeof(float), 0);
  overlay->bindUniform("model");
  overlay->bindUniform("projection");
  overlay->bindUniform("image");
  overlay->bindUniform("penColor");
  glUniformMatrix4fv(overlay->uniforms.at("model"), 1, false, glm::value_ptr(
    glm::scale(glm::mat4(1.0f), glm::vec3((float)width, (float)height, 1.0))
  ));
  glUniformMatrix4fv(overlay->uniforms.at("projection"), 1, false, glm::value_ptr(projection));

  overlay->textures.insert(Textures::namedTexture(
    "overlay", Textures::empty(width, height)
  ));
  Textures::texture* overlayBuff = overlay->textures.at("overlay");
  overlayBuff->penLine(floor(height / 2.0), overlayColor);

  objects.insert(object("triangle", triangle));
  objects.insert(object("overlay", overlay));
}

double oldTime = 0.0;
double rTime = 0.0;

void render() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  double time = glfwGetTime() / 4.0;
  rTime += ((time - oldTime) * 255);
  if (rTime > 255) rTime = 0;

  glUseProgram(*triangle->shader);

  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians((float)((rTime / 255) * 360.0)), glm::vec3(0,1,0));
  glUniformMatrix4fv(triangle->uniforms.at("model"), 1, false, glm::value_ptr(camera->Project(model)));

  GL::Vec4 colorVec(rTime / 255.0f, 100.0f / 255.0f, 200.0f / 255.0f, 1.0f);
  triangle->shader->SetUniform(triangle->uniforms.at("color"), colorVec);

  oldTime = time;

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  Element::draw(triangle->vao);

  glDisable(GL_DEPTH_TEST);

  // Render overlay 2D graphics surface

  //TODO: Make this configurable with script? ;)
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glUseProgram(*overlay->shader);

  glActiveTexture(GL_TEXTURE0);
  overlay->textures.at("overlay")->bind(0);

  colorVec = GL::Vec4(overlayColor.R / 255.0f, overlayColor.G / 255.0f, overlayColor.B / 255.0f, overlayColor.A / 255.0f);
  overlay->shader->SetUniform(overlay->uniforms.at("penColor"), colorVec);

  Element::draw(overlay->vao);

  glDisable(GL_BLEND);
}
