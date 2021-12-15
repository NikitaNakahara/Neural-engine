#pragma once

#include <GLFW/glfw3.h>
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.hpp"
#include "shader/ShaderProgram.hpp"
#include "models/ModelLoader.hpp"

namespace Graphic
{
	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		int init(unsigned int width, unsigned int height);

	private:
		void on_update(GLFWwindow* m_pWindow, ShaderProgram shaders[], Model models[], GLuint textures[], int width, int height);
		void shutdown();
		void bufferInit();
		GLuint texture, VBO, objectVAO, EBO, quadVAO, quadVBO, FBO, RBO;
		glm::vec3 lightPos = glm::vec3(2.f, 2.0f, 2.f);
		glm::mat4 lastView;
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 view1 = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 interfaceProjection = glm::mat4(1.0f);
		float scale[3] = { 1.f, 1.f, 1.f };
		float rotate[3] = { 0.f, 0.f, 0.f };
		float translate[3] = { 0.f, 0.f, 0.f };
		float diffuse[3] = { 0.5f, 0.5f, 0.5f };
		float shininess = 64.0f;
		float m_backgroundColor[4] = { 0.f, 0.f, 0.f, 0.f };
		bool invert = false;
		bool sepia = false;
		bool sharpnessCore = false;
		bool blurringCore = false;
		bool boardSharpnessCore = false;
		float rendererTextureScaleX = 1;
	};
}