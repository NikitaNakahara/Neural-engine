#include "ShaderReader.hpp"
#include "../../../Log.hpp"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Graphic
{
	ShaderReader::ShaderReader(const char* vertexName, const char* fragmentName)
	{
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			vShaderFile.open(vertexName);
			fShaderFile.open(fragmentName);
			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e)
		{
			LOG_ERROR("Filed open file");
		}
	}

	ShaderReader::~ShaderReader()
	{
	}
}