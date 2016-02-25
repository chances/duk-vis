#pragma once

#include <map>
#include <vector>

#include "ShaderFile.hpp"
#include <GL/OOGL.hpp>

using namespace std;

namespace Shaders {
  typedef pair<const char*, GLuint> uniform;

  GL::Program* bindShaders(const string& vertPath, const string& fragPath) {
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
}
