#pragma once

#include <GLFW/glfw3.h>
#include "shader/ShaderProgram.hpp"
#include <glm/glm.hpp>

namespace Graphic
{
	class Framebuffer
	{
	public:
		Framebuffer();
		~Framebuffer() {};

		void useTexture();
		GLuint getFBO() { return FBO; }
		GLuint getVAO() { return quadVAO; }

		void framebufferInit(float width, float height);
		void setMat4(const char* name, const glm::mat4 mat);
		void useFramebuffer();

	private:
		GLuint FBO, RBO, texture, quadVAO, quadVBO, shaderProgram;
		ShaderProgram shader;
	};
}