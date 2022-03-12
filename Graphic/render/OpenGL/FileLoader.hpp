#pragma once

#include <string>
#include "shader/ShaderProgram.hpp"
#include "models/ModelLoader.hpp"

namespace Graphic
{
	class FileLoader
	{
	public:
		FileLoader();
		
		ShaderProgram getShader(std::string name);
		Model getModel(std::string name);
	};
}