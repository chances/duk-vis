#pragma once

#include <OpenGL/gl3.h>
#include <GL/GL/Shader.hpp>
#include <exception>
#include <string>

#define GLSL( x ) "#version 330 core\n" #x

namespace GL {

	class ShaderFile : public Shader {
	public:
		ShaderFile(ShaderType::shader_type_t type);

		void SourceFromFile(const std::string& path);
	};
}
