#include "FileLoader.hpp"
#include "../../Log.hpp"


namespace Graphic
{
	FileLoader::FileLoader() {}

#ifdef NDEBUG
	Model FileLoader::getModel(std::string name)
	{
		return Model(std::string("res/models/" + name + "/" + name + ".obj").c_str());
}

	ShaderProgram FileLoader::getShader(std::string name)
	{
		return ShaderProgram(std::string("res\\shaders\\" + name + ".vs").c_str(), std::string("res\\shaders\\" + name + ".fs").c_str());
	}

#else
	Model FileLoader::getModel(std::string name)
	{
		return Model(std::string("../../res/models/" + name + "/" + name + ".obj").c_str());
	}

	ShaderProgram FileLoader::getShader(std::string name)
	{
		return ShaderProgram(std::string("..\\..\\res\\shaders\\" + name + ".vs").c_str(), std::string("..\\..\\res\\shaders\\" + name + ".fs").c_str());
	}
#endif
}