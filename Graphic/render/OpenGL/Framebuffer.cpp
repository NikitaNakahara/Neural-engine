#include <glad/glad.h>
#include <GLFW/glfw3.h>
 
#include "Framebuffer.hpp"
#include "../../Log.hpp"
#include "shader/ShaderProgram.hpp"
#include <glm/glm.hpp>


namespace Graphic
{
	Framebuffer::Framebuffer()
	{}

	void Framebuffer::framebufferInit(float width, float height)
	{
		float quadVertices[] = {
			-1.0f,  1.0f,   0.0f, 1.0f,
			-1.0f, -1.0f,   0.0f, 0.0f,
			 1.0f, -1.0f,   1.0f, 0.0f,

			-1.0f,  1.0f,   0.0f, 1.0f,
			 1.0f, -1.0f,   1.0f, 0.0f,
			 1.0f,  1.0f,   1.0f, 1.0f
		};

		glGenFramebuffers(1, &FBO);
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

		glGenRenderbuffers(1, &RBO);
		glBindRenderbuffer(GL_RENDERBUFFER, RBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

		auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
			LOG_ERROR("Framebuffer error: {0}", fboStatus);

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		shader = ShaderProgram("..\\..\\res\\shaders\\fbo.vs", "..\\..\\res\\shaders\\fbo.fs");
		shaderProgram = shader.getShaderProgram();
		glUseProgram(shaderProgram);
	}

	void Framebuffer::useFramebuffer()
	{
		glUseProgram(shaderProgram);
	}

	void Framebuffer::useTexture()
	{
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	void Framebuffer::setMat4(const char* name, const glm::mat4 mat)
	{
		shader.setMat4(name, mat);
	}
}