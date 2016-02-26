#pragma once

#include <string>
#include <map>
#include <vector>

#include "Shaders.hpp"
#include "Textures.hpp"
#include <GL/OOGL.hpp>

#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

using namespace std;

namespace Element {
  struct color {
    float r, g, b, a;
    color() : color(0.0f, 0.0f, 0.0f, 0.0f) {}
    color(int r, int g, int b) :
      color(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f) {}
    color(int r, int g, int b, float a) :
      color(r / 255.0f, g / 255.0f, b / 255.0f, a) {}
    color(float r, float g, float b) : color(r, g, b, 1.0f) {}
    color(float r, float g, float b, float a) {
      this->r = r;  this->g = g;  this->b = b;  this->a = a;
    }
    color(float* colorArr) {
      this->r = colorArr[0];  this->g = colorArr[1];
      this->b = colorArr[2];  this->a = colorArr[3];
    }
    operator GL::Color() const {
      return GL::Color(r * 255.0f, g * 255.0f, b * 255.0f, a * 255.0f);
    }
    operator float*() const {
      return (float*)this;
    }
  };

  struct vertices {
    int numIndices;
    float* verts;
    vertices(float* vertices) : numIndices(0) {
      verts = vertices;
    }
  };

  typedef pair<const GL::VertexArray*, int> vaoVertexCount;
  map<const GL::VertexArray*, int> vaoIndices;

  typedef pair<const GL::VertexBuffer*, vertices*> vboVertices;

  struct object {
    char* name = NULL;
    GL::Program* shader = NULL;
    GL::VertexArray* vao = NULL;
    vector<GL::VertexBuffer*> vbos;
    map<const GL::VertexBuffer*, vertices*> vboVertices;
    map<const char*, GLuint> uniforms;
    map<const char*, Textures::texture*> textures;
    object(char* name) : name(name) {};
    GL::Uniform bindUniform(char* name) {
      GL::Uniform uniform = shader->GetUniform(name);
      uniforms.insert(Shaders::uniform(name, uniform));
      return uniform;
    }
  };

  object* create(vertices* verts, object* prototype, int vertexIndexCount = 3,
      bool loadShaders = true, bool bindAttributes = true) {
    object* obj = prototype;

    if (loadShaders) {
      obj->shader = Shaders::bindShaders(
        string(obj->name).append(".vs.glsl"),
        string(obj->name).append(".fs.glsl")
      );
    }

    int vertexCount = verts->numIndices / vertexIndexCount;

    GL::VertexBuffer* vbo = new GL::VertexBuffer(
      verts->verts, sizeof(float)*verts->numIndices, GL::BufferUsage::StaticDraw
    );

    obj->vao = new GL::VertexArray();
    if (bindAttributes) {
      obj->vao->BindAttribute(
        obj->shader->GetAttribute("position"),
        *vbo, GL::Type::Float, vertexIndexCount, 0, 0
      );
    }

    obj->vbos.push_back(vbo);
    obj->vboVertices.insert(vboVertices(vbo, verts));

    // Add vao to index of vertex counts
    vaoIndices.insert(vaoVertexCount(obj->vao, vertexCount));

    return obj;
  }

  object* create(char* name, vertices* verts) {
    object* obj = new object(name);

    return create(verts, obj);
  }

  void draw(GL::VertexArray* vao) {
    glBindVertexArray(*vao);
    glDrawArrays(GL::Primitive::Triangle, 0, vaoIndices.at(vao));
  }
}
