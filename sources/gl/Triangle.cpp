#include <GL/GL/Context.hpp>
#include "Triangle.hpp"

namespace GL {
  Triangle::Triangle(Context* gl) {

    this->gl = gl;
  }

  Triangle::~Triangle() {
    delete vbo;
    delete vao;
    delete shader;
  }

  bool Triangle::BindShaders(const std::string &vertPath, const std::string &fragPath) {
    GL::ShaderFile* vertShader = new GL::ShaderFile(GL::ShaderType::Vertex);
    vertShader->SourceFromFile(vertPath);

    GL::ShaderFile* fragShader = new GL::ShaderFile(GL::ShaderType::Fragment);
    fragShader->SourceFromFile(fragPath);

    shader = new GL::Program(*vertShader, *fragShader);

    glDetachShader(*shader, *vertShader);
    glDetachShader(*shader, *fragShader);

    delete vertShader;
    delete fragShader;

    // Create buffers
    static const GLfloat vertices[] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f,  1.0f, 0.0f,
    };

    vbo = new GL::VertexBuffer( vertices, sizeof( vertices ), GL::BufferUsage::StaticDraw);

    vao = new GL::VertexArray();
    vao->BindAttribute(shader->GetAttribute( "position" ), *vbo, GL::Type::Float, 2, 0, 0);

    return true;
  }

  Program* Triangle::GetProgram() {
    return shader;
  }

  void Triangle::Draw() {
    glUseProgram(*shader);

    gl->DrawArrays(*vao, GL::Primitive::Triangle, 0, 3);
  }
}
