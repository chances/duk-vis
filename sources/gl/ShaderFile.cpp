#include "ShaderFile.hpp"
#include <iostream>
#include <fstream>
#include <vector>

namespace GL {
	ShaderFile::ShaderFile(ShaderType::shader_type_t shader ) : Shader(shader) {}

	void ShaderFile::SourceFromFile(const std::string &path) {
		std::string shaderCode;
		std::ifstream fileStream(path);
		if (fileStream.is_open()) {
			std::string line = "";
			while(getline(fileStream, line))
				shaderCode += "\n" + line;
			fileStream.close();
		}

		Source(shaderCode);
		Compile();
	}
}
