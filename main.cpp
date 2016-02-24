#include <iostream>

#include <GL/Window/Window.hpp>

#include <GLFW/glfw3.h>

using namespace std;

void windowRefresh(GLFWwindow* window);
void init();
void render();

GLFWwindow *window;

GL::Context* gl = NULL;
GL::VertexArray* vao = NULL;

int main() {

  if (!glfwInit())
    exit(EXIT_FAILURE);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  window = glfwCreateWindow(800, 600, "DukDukVis", NULL, NULL);
  if (!window) {
    glfwTerminate();
    exit(EXIT_FAILURE);
  }

  glfwSetWindowRefreshCallback(window, windowRefresh);

  glfwMakeContextCurrent(window);

  init();

  GL::Event ev;
  while (!glfwWindowShouldClose(window)) {

    render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  delete vao;

  glfwDestroyWindow(window);

  glfwTerminate();
  exit(EXIT_SUCCESS);
}

void windowRefresh(GLFWwindow* window) {
   render();
   glfwSwapBuffers(window);
}

void init() {
    GL::Shader vert( GL::ShaderType::Vertex, "#version 150\nin vec2 position; void main() { gl_Position = vec4( position, 0.0, 1.0 ); }" );
    GL::Shader frag( GL::ShaderType::Fragment, "#version 150\nout vec4 outColor; void main() { outColor = vec4( 1.0, 0.0, 0.0, 1.0 ); }" );
    GL::Program program( vert, frag );

    float vertices[] = {
            -0.5f,  0.5f,
            0.5f,  0.5f,
            0.5f, -0.5f
    };
    GL::VertexBuffer vbo( vertices, sizeof( vertices ), GL::BufferUsage::StaticDraw );

    vao = new GL::VertexArray;
    vao->BindAttribute( program.GetAttribute( "position" ), vbo, GL::Type::Float, 2, 0, 0 );
}

void render() {
    gl->Clear();
//    GL::Color clearColor = GL::Color(25, 25, 120);
//
//    gl->ClearColor(clearColor);

//    glBindVertexArray(*vao);
//    glDrawArrays(GL::Primitive::Triangle, 0, 3);
    gl->DrawArrays(*vao, GL::Primitive::Triangle, 0, 3 );
}
