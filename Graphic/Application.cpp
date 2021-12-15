#include <iostream>

#include "Application.hpp"
#include "render/OpenGL/Renderer.hpp"
#include "Log.hpp"

namespace Graphic
{
	Application::Application()
	{
		LOG_INFO("Start Application");
	}

	Application::~Application()
	{
	}


	int Application::start(unsigned int width, unsigned int height)
	{
		Renderer window = Renderer();
		int returnCode = window.init(width, height);
		return returnCode;
	}
}