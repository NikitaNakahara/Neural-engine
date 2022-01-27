#include "../Graphic/Application.hpp"

int main()
{
	Graphic::Application myApp = Graphic::Application();
	int returnCode = myApp.start(1920, 1080);
	return returnCode;
}