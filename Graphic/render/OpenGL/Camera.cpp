#include "Camera.hpp"
#include "../../Log.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>


float fow = 45.0f;

float lastX = 1920 / 0.2f, lastY = 1080 / 0.2f;
bool firstMouse = true;

namespace Graphic
{
	Camera::Camera()
	{}

	Camera::~Camera()
	{}

	glm::mat4 Camera::getView()
	{
		return glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	}

	void Camera::mouse_rotate(float xoffset, float yoffset)
	{
		float mouseSensetivity = 0.05f;
		xoffset *= mouseSensetivity;
		yoffset *= mouseSensetivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)	pitch = 89.0f;
		if (pitch < -89.0f)	pitch = -89.0f;

		glm::vec3 direction;
		direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
		direction.y = sin(glm::radians(pitch));
		direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
		cameraFront = glm::normalize(direction);
	}

	void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if (firstMouse)
		{
			lastX = xpos;
			lastY = ypos;
			firstMouse = false;
		}

		if (!mousePress)
		{
			lastX = xpos;
			lastY = ypos;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos;

		lastX = xpos;
		lastY = ypos;

		mouse_rotate(xoffset, yoffset);
	}

	void Camera::cameraMove(GLFWwindow* m_pWindow)
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		float cameraSpeed = 2.5f * deltaTime;

		if (glfwGetKey(m_pWindow, GLFW_KEY_W) == GLFW_PRESS)	cameraPos += cameraSpeed * cameraFront;
		if (glfwGetKey(m_pWindow, GLFW_KEY_S) == GLFW_PRESS)	cameraPos -= cameraSpeed * cameraFront;
		if (glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS)	cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (glfwGetKey(m_pWindow, GLFW_KEY_D) == GLFW_PRESS)	cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	void Camera::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{
			mousePress = true;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else 
		{ 
			mousePress = false;
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
}