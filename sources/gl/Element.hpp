#pragma once

#include <map>
#include <vector>

#include "ShaderFile.hpp"
#include <GL/OOGL.hpp>

#define ARRAY_SIZE(array) (sizeof((array))/sizeof((array[0])))

using namespace std;

namespace Element {
    typedef pair<const GL::VertexArray*, int> vaoVertexCount;
    map<const GL::VertexArray*, int> vaoVertices;

    struct vertices {
        int numVerts;
        float* verts;
        vertices(float* vertices) : numVerts(0) {
            verts = vertices;
        }
    };

    void create(GL::VertexArray*& vao, GL::VertexBuffer*& vbo, GL::Program* shader, vertices* verts) {
        int vertexCount = verts->numVerts / 3;

        vbo = new GL::VertexBuffer(verts->verts, sizeof(float)*verts->numVerts, GL::BufferUsage::StaticDraw);

        vao = new GL::VertexArray();
        vao->BindAttribute(shader->GetAttribute("position"), *vbo, GL::Type::Float, 3, 0, 0);

        // Add vao to index of vertex counts
        vaoVertices.insert(vaoVertexCount(vao, vertexCount));
    }

    GL::Program* bindShaders(const std::string& vertPath, const std::string& fragPath) {
      GL::ShaderFile* vertShader = new GL::ShaderFile(GL::ShaderType::Vertex);
      vertShader->SourceFromFile(vertPath);

      GL::ShaderFile* fragShader = new GL::ShaderFile(GL::ShaderType::Fragment);
      fragShader->SourceFromFile(fragPath);

      GL::Program* shader = new GL::Program(*vertShader, *fragShader);

      glDetachShader(*shader, *vertShader);
      glDetachShader(*shader, *fragShader);

      delete vertShader;
      delete fragShader;

      return shader;
    }

    void draw(GL::VertexArray* vao) {
        glBindVertexArray(*vao);
        glDrawArrays(GL::Primitive::Triangle, 0, vaoVertices.at(vao));
    }
}
