#pragma once

#include <GLFW/glfw3.h>
#include <tuple>

#include "stb_image.h"

namespace Graphic
{
	class MTexture
	{
	public:
		MTexture(const char* path, GLenum type, GLenum color);
		~MTexture();

		GLuint getTexture() { return texture; }
	private:
		GLuint texture;
	};
}