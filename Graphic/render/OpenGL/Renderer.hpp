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
	struct SModel
	{
		ShaderProgram shader;
		GLuint shader_program;
		Model model;
		glm::mat4 model_mat;
		std::string name;
		float position[3] = {0.0f};
		float scale[3] = {1.0f};
		float rotate[3] = {0.0f};
		bool draw = true;
		bool selected = false;
	};

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		int init(unsigned int width, unsigned int height);

	private:
		void on_update(GLFWwindow* m_pWindow, int width, int height);
		void shutdown();
		void bufferInit();
		GLuint texture, VBO, objectVAO, EBO, quadVAO, quadVBO, FBO, RBO;
		glm::vec3 lightPos = glm::vec3(1000000.f, 10000000.0f, 10000000.f);
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
		bool GUIModels = false;
		bool boardSharpnessCore = false;
		float rendererTextureScaleX = 1;
		std::vector<SModel> allModels;

		std::vector<SModel> models;
	};
}