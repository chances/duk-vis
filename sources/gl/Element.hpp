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
  struct vertices {
    int numVerts;
    float* verts;
    vertices(float* vertices) : numVerts(0) {
      verts = vertices;
    }
  };

  typedef pair<const GL::VertexArray*, int> vaoVertexCount;
  map<const GL::VertexArray*, int> vaoVertices;

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

  object* create(vertices* verts, object* prototype, int indexCount = 3,
      bool loadShaders = true, bool bindAttributes = true) {
    object* obj = prototype;

    if (loadShaders) {
      obj->shader = Shaders::bindShaders(
        string(obj->name).append(".vs.glsl"),
        string(obj->name).append(".fs.glsl")
      );
    }

    int vertexCount = verts->numVerts / indexCount;

    GL::VertexBuffer* vbo = new GL::VertexBuffer(
      verts->verts, sizeof(float)*verts->numVerts, GL::BufferUsage::StaticDraw
    );

    obj->vao = new GL::VertexArray();
    if (bindAttributes) {
      obj->vao->BindAttribute(
        obj->shader->GetAttribute("position"), *vbo, GL::Type::Float, 3, 0, 0
      );
    }

    obj->vbos.push_back(vbo);
    obj->vboVertices.insert(vboVertices(vbo, verts));

    // Add vao to index of vertex counts
    vaoVertices.insert(vaoVertexCount(obj->vao, vertexCount));

    return obj;
  }

  object* create(char* name, vertices* verts) {
    object* obj = new object(name);

    return create(verts, obj);
  }

  void draw(GL::VertexArray* vao) {
    glBindVertexArray(*vao);
    glDrawArrays(GL::Primitive::Triangle, 0, vaoVertices.at(vao));
  }
}
