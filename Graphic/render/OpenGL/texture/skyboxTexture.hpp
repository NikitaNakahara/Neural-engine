#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "../FileLoader.hpp"
#include <vector>
#include <string>


namespace Graphic
{
	class Skybox
	{
	public:
		Skybox(std::string name);
		~Skybox();

		GLuint initSkybox(std::vector<std::string> faces);
		void drawSkybox(glm::mat4 view, glm::mat4 projection);

		GLuint texture;

	private:
		GLuint shader;
		GLuint skyboxVAO, VBO;
		ShaderProgram skyboxShader = ShaderProgram();
		FileLoader skyboxLoader = FileLoader();
		bool loaded = false;
		std::string m_name;
	};
}