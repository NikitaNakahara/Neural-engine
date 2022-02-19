#pragma comment(lib, "d3d10.lib")

#include <Windows.h>

#include "../Graphic/Application.hpp"

int wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR nCmdLine, int nCmdShow)
{
	Graphic::Application myApp = Graphic::Application(hInstance, nCmdLine, nCmdShow);
	int returnCode = myApp.start(1920, 1080);
	return returnCode;
}