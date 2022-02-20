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
#include "FileLoader.hpp"

namespace Graphic
{
	struct SShader
	{
		ShaderProgram shader;
		GLuint shaderProgram;
		std::string name;
		bool selected = false;
	};

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
		SShader shader;
	};

	struct SSkybox
	{
		std::string name;
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
		void deleteModel(std::vector<SModel> models);
		void askCloseWindow(GLFWwindow* m_pWindow);
		void drawSkybox(std::vector<SSkybox> skyboxes);
		void drawModels(std::vector<SModel> models, glm::vec3 cameraPos, glm::vec3 cameraFront);
		void selectModel(GLFWwindow* m_pWindow, std::vector<SModel> models);
		void addNewModel();
		void sceneObjects(std::vector<SModel> models);
		void modelParameters(GLFWwindow* m_pWindow, std::vector<SModel> models);
		void addNewShader(GLFWwindow* m_pWindow);
		void selectSkybox(std::vector<SSkybox> skyboxes);

		void initModels();
		void on_update(GLFWwindow* m_pWindow, std::vector<SModel> models, std::vector<SSkybox> skyboxes, int width, int height);
		void shutdown();
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
		bool newShader = false;
		bool newModel = false;
		bool modelsGUI = false;
		std::string newShaderPath = "";
		std::string newModelPath = "";
		std::string newName;

		std::vector<SModel> simpleModels;
		std::vector<SShader> shaders;

		Resource::FileLoader fileLoader = Resource::FileLoader();
	};
}