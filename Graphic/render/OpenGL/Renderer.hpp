#pragma once

#include <GLFW/glfw3.h>
#include <tuple>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.hpp"
#include "shader/ShaderProgram.hpp"
#include "models/ModelLoader.hpp"
#include "texture/skyboxTexture.hpp"

namespace Graphic
{
	struct SModel
	{
		std::string name;
		Model model;
		glm::mat4 model_matrix = glm::mat4(1.0f);
		float translate[3] = {};
		float scale[3] = { 1.0f, 1.0f, 1.0f };
		float rotate[3] = {};
		bool draw = true;
		bool GUIselect = false;
		bool makeModel = false;
		bool mDelete = false;
		int count = 0;
	};

	struct SSkybox
	{
		std::string name;
		std::string path;
		Skybox skybox;
		bool draw;
	};

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		int init(unsigned int width, unsigned int height);

	private:
		void initModels();
		void on_update(GLFWwindow* m_pWindow, ShaderProgram shaders[], std::vector<SModel> models, std::vector<SSkybox> skyboxes, GLuint textures[], int width, int height);
		void shutdown();
		void bufferInit();
		GLuint texture, VBO, objectVAO, EBO, quadVAO, quadVBO, FBO, RBO;
		glm::vec3 lightPos = glm::vec3(100.f, 40.0f, 70.f);
		glm::mat4 lastView;
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 view1 = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 interfaceProjection = glm::mat4(1.0f);
		float diffuse[3] = { 0.5f, 0.5f, 0.5f };
		float lightColor[3] = { 1.f, 1.f, 1.f };
		float shininess = 64.0f;
		float m_backgroundColor[4] = { 0.f, 0.f, 0.f, 0.f };
		float rendererTextureScaleX = 1;
		bool not_pressed = true;

		std::vector<SModel> simpleModels;
	};
}