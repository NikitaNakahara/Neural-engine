#pragma once

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

namespace Graphic
{
	class Camera
	{
	public:
		Camera();
		~Camera();

		glm::mat4 getView();
		void cameraMove(GLFWwindow* m_pWindow);
		void mouse_callback(GLFWwindow* window, double xpos, double ypos);
		void mouse_rotate(float xoffset, float yoffset);
		void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

		glm::vec3 getCameraPos() { return cameraPos; }
		glm::vec3 getCameraFront() { return cameraFront; }
		float yaw = -90.0f;

	private:
		float pitch = 0.0f;
		float deltaTime				= 0.0f;
		float lastFrame				= 0.0f;
		glm::vec3 cameraPos			= glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 cameraUp			= glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraFront		= glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 cameraTarget		= glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 cameraDirection	= glm::normalize(cameraPos - cameraTarget);
		glm::vec3 up				= glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 cameraRight		= glm::normalize(glm::cross(up, cameraDirection));
		bool mousePress				= false;
	};
}
