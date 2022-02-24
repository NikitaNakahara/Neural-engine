#pragma once

namespace Graphic
{
	class Application
	{
	public:
		Application();
		~Application();

		int start(unsigned int width, unsigned int height);
	};
}