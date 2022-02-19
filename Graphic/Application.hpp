#pragma once

#include <string>
#include <Windows.h>

namespace Graphic
{
	class Application
	{
	public:
		Application(HINSTANCE hInstance, PWSTR nCmdLine, int nCmdShow);
		~Application();

		int start(unsigned int width, unsigned int height);
		
		int initWindow(std::string className, std::string wndName);
		int run();

	private:
		MSG m_msg;
		HWND m_hWnd;
		int m_nCmdShow;
		HINSTANCE m_hInstance;
	};
}