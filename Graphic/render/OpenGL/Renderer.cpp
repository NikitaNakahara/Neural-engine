#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <fstream>
#include <iostream>
#include <tuple>
#include <array>
#include <vector>

#include "Renderer.hpp"
#include "Camera.hpp"
#include "texture/TextureInit.hpp"
#include "../../Log.hpp"
#include "shader/ShaderProgram.hpp"
#include "models/ModelLoader.hpp"
#include "Framebuffer.hpp"
#include "texture/skyboxTexture.hpp"


namespace Graphic
{
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	Camera camera = Camera();
	Framebuffer FBOrender = Framebuffer();

	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
		shutdown();
	}
	
	void Renderer::initModels()
	{
		Model mCone = Model("../../res/models/cone/cone.obj");
		SModel cone = { "cone", mCone };
		Model mCube = Model("../../res/models/cube/cube.obj");
		SModel cube = { "cube", mCube };
		Model mCylinder = Model("../../res/models/cylinder/cylinder.obj");
		SModel cylinder = { "cylinder", mCylinder };
		Model mSphere = Model("../../res/models/sphere/sphere.obj");
		SModel sphere = { "sphere", mSphere };
		Model mTorus = Model("../../res/models/torus/torus.obj");
		SModel torus = { "torus", mTorus };

		simpleModels = { cone, cube, cylinder, sphere, torus };
	}

	void Renderer::on_update(GLFWwindow* m_pWindow, ShaderProgram shaders[], std::vector<SModel> models, std::vector<SSkybox> skyboxes, GLuint textures[], int width, int height)
	{
		glm::vec3 windows[] = {
			glm::vec3(0.f, 0.f, 3.f),
			glm::vec3(0.5f, 0.5f, 2.f)
		};

		ShaderProgram object_shader_program = shaders[0];
		GLint objectShaderProgram = object_shader_program.getShaderProgram();
		Model mHeart = Model("../../res/models/heart/heart.obj");
		SModel heart = { "heart", mHeart };

		bool cursorVisible = false;
		bool cursor = true;
		bool firstFrame = true;
		bool love = false;
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		LOG_INFO("Program was started");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glEnable(GL_MULTISAMPLE);

		while (!glfwWindowShouldClose(m_pWindow))
		{
			if (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(m_pWindow, true);

			if (glfwGetKey(m_pWindow, GLFW_KEY_L) == GLFW_PRESS &&
				glfwGetKey(m_pWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			{
				if (!love)
				{
					simpleModels.push_back(heart);
					love = true;
				}
			}

			camera.cameraMove(m_pWindow);

			glm::vec3 cameraPos = camera.getCameraPos();
			glm::vec3 cameraFront = camera.getCameraFront();

			projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

			glClearColor(m_backgroundColor[0], m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3]);
			model = glm::mat4(1.0f);

			view = camera.getView();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);

			for (int i = 0; i < skyboxes.size(); i++)
			{
				if (skyboxes[i].draw)
					skyboxes[i].skybox.drawSkybox(view, projection, skyboxes[i].path);
			}

			glDepthFunc(GL_LESS);

			glUseProgram(objectShaderProgram);
			object_shader_program.setMat4("view", view);
			object_shader_program.setMat4("projection", projection);
			object_shader_program.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
			object_shader_program.setVec3("lightColor", lightColor[0], lightColor[1], lightColor[2]);
			object_shader_program.setVec3("viewDir", cameraFront.x, cameraFront.y, cameraFront.z);
			object_shader_program.setVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
			object_shader_program.setVec3("lightAmbient", 0.2f, 0.2f, 0.2f);
			object_shader_program.setVec3("lightDiffuse", 0.5f, 0.5f, 0.5f);
			object_shader_program.setVec3("lightSpecular", 1.0f, 1.0f, 1.0f);

			for (int i = 0; i < models.size(); i++)
			{
				if (models[i].mDelete)
				{
					models.erase(models.begin() + i);
				}
			}
			
			for (int i = 0; i < models.size(); i++)
			{
				models[i].model_matrix = glm::mat4(1.0f);
				models[i].model_matrix = glm::translate(models[i].model_matrix, glm::vec3(models[i].translate[0], models[i].translate[1], models[i].translate[2]));
				models[i].model_matrix = glm::rotate(models[i].model_matrix, models[i].rotate[0],  glm::vec3(1.0f, 0.0f, 0.0f));
				models[i].model_matrix = glm::rotate(models[i].model_matrix, models[i].rotate[1],  glm::vec3(0.0f, 1.0f, 0.0f));
				models[i].model_matrix = glm::rotate(models[i].model_matrix, models[i].rotate[2],  glm::vec3(0.0f, 0.0f, 1.0f));
				models[i].model_matrix = glm::scale(models[i].model_matrix, glm::vec3(models[i].scale[0], models[i].scale[1], models[i].scale[2]));
				object_shader_program.setMat4("model", models[i].model_matrix);
				if (models[i].draw)
				{
					models[i].model.Draw(object_shader_program);
				}
			}
			
			for (int i = 0; i < simpleModels.size(); i++)
			{
				if (simpleModels[i].makeModel)
				{
					models.push_back(simpleModels[i]);
					if (models[models.size() - 1].name == simpleModels[i].name)
					{
						simpleModels[i].count++;
						models[models.size() - 1].name += std::to_string(simpleModels[i].count);
					}
					simpleModels[i].makeModel = false;
				}

				ImGuiIO& io = ImGui::GetIO();
				io.DisplaySize.x = static_cast<float>(width);
				io.DisplaySize.y = static_cast<float>(height);


				ImGui_ImplOpenGL3_NewFrame();
				ImGui_ImplGlfw_NewFrame();
				ImGui::NewFrame();

				if (glfwGetKey(m_pWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
					glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS)
				{
					if (ImGui::Begin("Models"))
					{
						for (int i = 0; i < simpleModels.size(); i++)
						{
							ImGui::Selectable(simpleModels[i].name.c_str(), &simpleModels[i].makeModel, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						}
						ImGui::End();
					}
				}

				if (ImGui::Begin("Objects"))
				{
					for (int i = 0; i < models.size(); i++)
					{
						ImGui::Checkbox(models[i].name.c_str(), &models[i].GUIselect);
						if (models[i].GUIselect)
						{
							for (int j = 0; j < models.size(); j++)
							{
								if (j != i)
									models[j].GUIselect = false;
							}
						}
					}
					ImGui::End();
				}


				if (ImGui::Begin("Transformation"))
				{
					for (int i = 0; i < models.size(); i++)
					{
						if (models[i].GUIselect)
						{
							ImGui::Selectable(models[i].name.c_str(), true, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
							ImGui::SliderFloat3("translate", models[i].translate, -3, 3);
							ImGui::SliderFloat3("rotate", models[i].rotate, -6.28, 6.28);
							ImGui::SliderFloat3("scale", models[i].scale, 0, 5);
							ImGui::Checkbox("draw", &models[i].draw);
							ImGui::Checkbox("delete", &models[i].mDelete);
							if (glfwGetKey(m_pWindow, GLFW_KEY_DELETE) == GLFW_PRESS)
								models[i].mDelete = true;
						}
					}
					ImGui::End();
				}

				if (ImGui::Begin("Skybox"))
				{
					for (int i = 0; i < skyboxes.size(); i++)
					{
						ImGui::Checkbox(skyboxes[i].name.c_str(), &skyboxes[i].draw);
						if (skyboxes[i].draw)
						{
							for (int j = 0; j < skyboxes.size(); j++)
							{
								if (j != i)
									skyboxes[j].draw = false;
							}
						}
					}
					ImGui::End();
				}


				ImGui::Render();
				ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

				glfwPollEvents();
				glfwSwapBuffers(m_pWindow);
			}
		}
	}

	int Renderer::init(unsigned int width, unsigned int height)
	{
		if (!glfwInit())
		{
			LOG_CRITICAL("Filed Initialize GLFW");
			return -1;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		GLFWwindow* m_pWindow = glfwCreateWindow(width, height, "Test", nullptr, nullptr);
		if (!m_pWindow)
		{
			LOG_CRITICAL("Filed Create Window");
			return -2;
		}
		glfwMakeContextCurrent(m_pWindow);

		glfwSetFramebufferSizeCallback(m_pWindow,
			[](GLFWwindow* window, int width, int height)
			{
				glViewport(0, 0, width, height);
			}
		);
		glfwSetCursorPosCallback(m_pWindow, mouse_callback);
		glfwSetMouseButtonCallback(m_pWindow, mouse_button_callback);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			LOG_CRITICAL("Filed Initialize GLAD");
			return -3;
		}

		stbi_set_flip_vertically_on_load(true);
		
		ShaderProgram object_shader = ShaderProgram("..\\..\\res\\shaders\\object.vs", "..\\..\\res\\shaders\\object.fs");
		ShaderProgram shaders[] = { object_shader };

		std::vector<SModel> models = {};

		MTexture texture1("..\\..\\res\\textures\\texture.png", GL_TEXTURE_2D, GL_RGBA);
		GLuint textures[] = { texture1.getTexture() };

		Skybox sea_skybox = Skybox();
		SSkybox sea_sskybox = { "sea", "..\\..\\res\\textures\\skybox\\", sea_skybox };
		Skybox yoko_skybox = Skybox();
		SSkybox yoko_sskybox = { "yokohama", "..\\..\\res\\textures\\yokohama\\", yoko_skybox };
		Skybox yoko_night_skybox = Skybox();
		SSkybox yoko_night_sskybox = { "yokohama_night", "..\\..\\res\\textures\\yokohama_night\\", yoko_night_skybox };
		Skybox storforsen_skybox = Skybox();
		SSkybox storforsen_sskybox = { "storforsen", "..\\..\\res\\textures\\storforsen\\", storforsen_skybox };

		std::vector<SSkybox> skyboxes = { sea_sskybox, yoko_sskybox, yoko_night_sskybox, storforsen_sskybox };

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init();
		ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);
		LOG_INFO("Loading: ImGui loaded");

		initModels();

		on_update(m_pWindow, shaders, models, skyboxes, textures, width, height);

		return 0;
	}

	void Renderer::shutdown()
	{
		glfwTerminate();
	}

	void mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		camera.mouse_callback(window, xpos, ypos);
	}

	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
	{
		camera.mouse_button_callback(window, button, action, mods);
	}
}