#pragma once

#include <string>
#include <map>
#include <vector>

#include "Shaders.hpp"
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
        object(char* name) : name(name) {};
        GL::Uniform bindUniform(char* name) {
            GL::Uniform uniform = shader->GetUniform(name);
            uniforms.insert(Shaders::uniform(name, uniform));
            return uniform;
        }
    };

    object* create(char* name, vertices* verts) {
        object* obj = new object(name);

        obj->shader = Shaders::bindShaders(string(name).append(".vs.glsl"), string(name).append(".fs.glsl"));

        int vertexCount = verts->numVerts / 3;

        GL::VertexBuffer* vbo = new GL::VertexBuffer(verts->verts, sizeof(float)*verts->numVerts, GL::BufferUsage::StaticDraw);

        obj->vao = new GL::VertexArray();
        obj->vao->BindAttribute(obj->shader->GetAttribute("position"), *vbo, GL::Type::Float, 3, 0, 0);

        obj->vbos.push_back(vbo);

        // Add vao to index of vertex counts
        vaoVertices.insert(vaoVertexCount(obj->vao, vertexCount));

        return obj;
    }

    void draw(GL::VertexArray* vao) {
        glBindVertexArray(*vao);
        glDrawArrays(GL::Primitive::Triangle, 0, vaoVertices.at(vao));
    }
}