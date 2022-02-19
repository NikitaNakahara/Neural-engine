#include "FileLoader.hpp"


namespace Resource
{
	FileLoader::FileLoader() {}

	Graphic::ShaderProgram FileLoader::getShader(std::string path)
	{
		std::string fullPath = std::string("res\\shaders\\" + path);
		return Graphic::ShaderProgram(std::string(fullPath + ".vs").c_str(), std::string(fullPath + ".fs").c_str());
	}

	Graphic::Model FileLoader::getModel(std::string path)
	{
			return Graphic::Model(std::string("res\\shaders\\" + path).c_str());
	}

	Graphic::MTexture FileLoader::getTexture(std::string path)
	{
		return Graphic::MTexture(std::string("res\\textures\\" + path).c_str(), NULL, NULL);
	}

	Graphic::Skybox FileLoader::getSkybox(std::string path)
	{
		return Graphic::Skybox(std::string("res\\textures\\" + path + "\\"));
	}
}