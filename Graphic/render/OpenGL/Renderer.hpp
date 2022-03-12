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
		ShaderProgram shaderObj;
		GLuint shader;
		std::string name;
		bool select = false;
	};

	struct SModel
	{
		ShaderProgram shader;
		GLuint shader_program;
		std::string shaderName;
		Model model;
		glm::mat4 model_mat;
		std::string name;
		float position[3] = {0.0f};
		float scale[3] = { 1.0f, 1.0f, 1.0f };
		float rotate[3] = {0.0f};
		bool draw = true;
		bool selected = false;
		bool delete_mdl = false;
		int count = 0;
	};

	struct SSkybox
	{
		Skybox skybox;
		std::string name;
		bool draw = false;
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
		void initModels();
		void initSkybox();
		GLuint texture, VBO, objectVAO, EBO, quadVAO, quadVBO, FBO, RBO;
		glm::vec3 lightPos = glm::vec3(1000000.f, 10000000.0f, 10000000.f);
		glm::mat4 lastView;
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 view1 = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 interfaceProjection = glm::mat4(1.0f);
		float diffuse[3] = { 0.5f, 0.5f, 0.5f };
		float shininess = 64.0f;
		float m_backgroundColor[4] = { 0.f, 0.f, 0.f, 0.f };
		bool invert = false;
		bool sepia = false;
		bool sharpnessCore = false;
		bool blurringCore = false;
		bool GUIModels = false;
		bool boardSharpnessCore = false;
		bool newModel = false;
		bool newSkybox = false;
		bool selectShader = false;
		char newModelPath[64] = "";
		char newSkyboxPath[64] = "";
		float rendererTextureScaleX = 1;
		std::vector<SModel> allModels;
		std::vector<SModel> models;
		std::vector<SSkybox> skyboxes;
		std::vector<SShader> shaders;
		FileLoader fileLoader = FileLoader();
		bool p_true = true;
	};
}