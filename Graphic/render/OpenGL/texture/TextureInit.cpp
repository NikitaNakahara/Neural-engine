#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"
#include "TextureInit.hpp"

namespace Graphic
{
	MTexture::MTexture(const char* path, GLenum type, GLenum color)
	{
		int width, height, nrChannels;
		glGenTextures(1, &texture);
		glBindTexture(type, texture);

		unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(type, 0, color, width, height, 0, color, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(type);
		}
		else
		{
		}
		stbi_image_free(data);

		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
		

	MTexture::~MTexture()
	{}
}