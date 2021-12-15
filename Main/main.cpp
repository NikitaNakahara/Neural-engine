#include "../Graphic/Application.hpp"

int main()
{
	Graphic::Application myApp = Graphic::Application();
	int returnCode = myApp.start(1024, 768);
	return returnCode;
}