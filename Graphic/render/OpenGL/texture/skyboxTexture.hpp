#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "../shader/ShaderProgram.hpp"

#include <vector>
#include <string>


namespace Graphic
{
	class Skybox
	{
	public:
		Skybox();
		~Skybox();

		GLuint initSkybox(std::vector<std::string> faces);
		void drawSkybox(glm::mat4 view, glm::mat4 projection);

		GLuint texture;

	private:
		GLuint shader;
		GLuint skyboxVAO, VBO;
		ShaderProgram skyboxShader = ShaderProgram();
		bool loaded = false;
	};
}