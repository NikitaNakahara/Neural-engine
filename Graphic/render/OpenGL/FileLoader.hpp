#pragma once

#include "shader/ShaderProgram.hpp"
#include "texture/TextureInit.hpp"
#include "texture/skyboxTexture.hpp"
#include "models/ModelLoader.hpp"

#include <string>

namespace Resource
{
	class FileLoader
	{
	public:
		FileLoader();

		Graphic::ShaderProgram getShader(std::string path);
		Graphic::MTexture getTexture(std::string path);
		Graphic::Skybox getSkybox(std::string path);
		Graphic::Model getModel(std::string path);
	};
}