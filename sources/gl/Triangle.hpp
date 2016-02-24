#pragma once

#include <OpenGL/gl3.h>
#include <string>

#include "ShaderFile.hpp"
#include <GL/GL/Context.hpp>
#include <GL/GL/VertexArray.hpp>
#include <GL/GL/Program.hpp>

namespace GL {
    class Triangle {
    public:
        Triangle(Context* gl);
        ~Triangle();

        bool BindShaders(const std::string& vertPath, const std::string& fragPath);
        Program* GetProgram();
        void Draw();
    private:
        Context* gl;
        VertexArray* vao = 0;
        VertexBuffer* vbo;
        Program* shader;
    };
}
