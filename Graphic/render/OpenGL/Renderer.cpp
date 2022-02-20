#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <imgui/backends/imgui_impl_glfw.h>

#include <Windows.h>

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
#include "texture/skyboxTexture.hpp"


namespace Graphic
{
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	Camera camera = Camera();

	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
		shutdown();
	}
	
	void Renderer::initModels()
	{
		Model mCone = fileLoader.getModel("cone.obj");
		SModel cone = { "cone", mCone };
		cone.shader = shaders[0];
		Model mCube = fileLoader.getModel("cube.obj");
		SModel cube = { "cube", mCube };
		cube.shader = shaders[0];
		Model mCylinder = fileLoader.getModel("cylinder.obj");
		SModel cylinder = { "cylinder", mCylinder };
		cylinder.shader = shaders[0];
		Model mSphere = fileLoader.getModel("sphere.obj");
		SModel sphere = { "sphere", mSphere };
		sphere.shader = shaders[0];
		Model mTorus = fileLoader.getModel("torus.obj");
		SModel torus = { "torus", mTorus };
		torus.shader = shaders[0];

		simpleModels = { cone, cube, cylinder, sphere, torus };
	}

	void Renderer::on_update(GLFWwindow* m_pWindow, std::vector<SModel> models, std::vector<SSkybox> skyboxes, int width, int height)
	{
		Model mHeart = fileLoader.getModel("heart.obj");
		SModel heart = { "heart", mHeart };

		bool cursorVisible = false;
		bool cursor = true;
		bool firstFrame = true;
		bool love = false;
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		glEnable(GL_MULTISAMPLE);

		while (!glfwWindowShouldClose(m_pWindow))
		{
			askCloseWindow(m_pWindow);

			glClearColor(m_backgroundColor[0], m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3]);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);


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
			view = camera.getView();
			projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

			drawSkybox(skyboxes);
			deleteModel(models);
			drawModels(
				models,
				camera.getCameraPos(),
				camera.getCameraFront()
			
			);
			selectModel(m_pWindow, models);
			addNewModel();
			addNewShader(m_pWindow);
			sceneObjects(models);
			modelParameters(m_pWindow, models);
			selectSkybox(skyboxes);


			ImGuiIO& io = ImGui::GetIO();
			io.DisplaySize.x = static_cast<float>(width);
			io.DisplaySize.y = static_cast<float>(height);


			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwPollEvents();
			glfwSwapBuffers(m_pWindow);
		}
	}

	int Renderer::init(unsigned int width, unsigned int height)
	{
		if (!glfwInit())
		{
			LOG_CRITICAL("Filed Initialize GLFW");
			PostQuitMessage(E_FAIL);
			return -1;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_SAMPLES, 4);

		GLFWwindow* m_pWindow = glfwCreateWindow(width, height, "Test", nullptr, nullptr);
		if (!m_pWindow)
		{
			MessageBox(NULL, "Filed Create Window", "Error", MB_ICONERROR | MB_OK);
			PostQuitMessage(E_FAIL);
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
			MessageBox(NULL, "Filed Init Glad", "Error", MB_ICONERROR | MB_OK);
			PostQuitMessage(E_FAIL);
			return -3;
		}

		stbi_set_flip_vertically_on_load(true);
		
		ShaderProgram object_shader = fileLoader.getShader("object");
		SShader shader;
		shader.name = "diffuse_shader";
		shader.shader = object_shader;
		shader.shaderProgram = object_shader.getShaderProgram();
		shaders.push_back(shader);

		std::vector<SModel> models = {};

		Skybox sea_skybox = fileLoader.getSkybox("skybox");
		SSkybox sea_sskybox = { "sea", sea_skybox };
		Skybox yoko_skybox = fileLoader.getSkybox("yokohama");
		SSkybox yoko_sskybox = { "yokohama", yoko_skybox };
		Skybox yoko_night_skybox = fileLoader.getSkybox("yokohama_night");
		SSkybox yoko_night_sskybox = { "yokohama_night", yoko_night_skybox };
		Skybox storforsen_skybox = fileLoader.getSkybox("storforsen");
		SSkybox storforsen_sskybox = { "storforsen", storforsen_skybox };

		std::vector<SSkybox> skyboxes = { sea_sskybox, yoko_sskybox, yoko_night_sskybox, storforsen_sskybox };

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init();
		ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);

		initModels();

		on_update(m_pWindow, models, skyboxes, width, height);

		return 0;
	}

	void Renderer::deleteModel(std::vector<SModel> models)
	{
		for (int i = 0; i < models.size(); i++)
		{
			if (models[i].mDelete)
			{
				models.erase(models.begin() + i);
			}
		}
	}

	void Renderer::drawSkybox(std::vector<SSkybox> skyboxes)
	{
		for (int i = 0; i < skyboxes.size(); i++)
		{
			if (skyboxes[i].draw)
				skyboxes[i].skybox.drawSkybox(view, projection);
		}
		glDepthFunc(GL_LESS);
	}

	void Renderer::drawModels(std::vector<SModel> models, glm::vec3 cameraPos, glm::vec3 cameraFront)
	{
		for (int i = 0; i < models.size(); i++)
		{
			glUseProgram(models[i].shader.shaderProgram);
			models[i].shader.shader.setMat4("view", view);
			models[i].shader.shader.setMat4("projection", projection);
			models[i].shader.shader.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
			models[i].shader.shader.setVec3("lightColor", lightColor[0], lightColor[1], lightColor[2]);
			models[i].shader.shader.setVec3("viewDir", cameraFront.x, cameraFront.y, cameraFront.z);
			models[i].shader.shader.setVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
			models[i].shader.shader.setVec3("lightAmbient", 0.2f, 0.2f, 0.2f);
			models[i].shader.shader.setVec3("lightDiffuse", 0.5f, 0.5f, 0.5f);
			models[i].shader.shader.setVec3("lightSpecular", 1.0f, 1.0f, 1.0f);

			models[i].model_matrix = glm::mat4(1.0f);
			models[i].model_matrix = glm::translate(models[i].model_matrix, glm::vec3(models[i].translate[0], models[i].translate[1], models[i].translate[2]));
			models[i].model_matrix = glm::rotate(models[i].model_matrix, models[i].rotate[0], glm::vec3(1.0f, 0.0f, 0.0f));
			models[i].model_matrix = glm::rotate(models[i].model_matrix, models[i].rotate[1], glm::vec3(0.0f, 1.0f, 0.0f));
			models[i].model_matrix = glm::rotate(models[i].model_matrix, models[i].rotate[2], glm::vec3(0.0f, 0.0f, 1.0f));
			models[i].model_matrix = glm::scale(models[i].model_matrix, glm::vec3(models[i].scale[0], models[i].scale[1], models[i].scale[2]));
			models[i].shader.shader.setMat4("model", models[i].model_matrix);
			if (models[i].draw)
			{
				models[i].model.Draw(models[i].shader.shader);
			}
		}
	}

	void Renderer::selectModel(GLFWwindow* m_pWindow, std::vector<SModel> models)
	{
		for (int i = 0; i < simpleModels.size(); i++)
		{

			if (glfwGetKey(m_pWindow, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS &&
				glfwGetKey(m_pWindow, GLFW_KEY_A) == GLFW_PRESS)
				modelsGUI = true;

			if (modelsGUI)
			{
				if (ImGui::Begin("Models"))
				{
					for (int i = 0; i < simpleModels.size(); i++)
					{
						ImGui::Selectable(simpleModels[i].name.c_str(), &simpleModels[i].makeModel, ImGuiSelectableFlags_::ImGuiSelectableFlags_DontClosePopups);
						if (simpleModels[i].makeModel || glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE))
							modelsGUI = false;
					}
					if (ImGui::Button("New model"))
						newModel = true;
					ImGui::End();
				}
			}

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
		}
	}

	void Renderer::addNewModel()
	{
		if (newModel)
		{
			if (ImGui::Begin("New model"))
			{
				ImGui::InputText("", const_cast<char*>(newModelPath.c_str()), 64);

				if (ImGui::Button("Add") && !newModelPath.empty())
				{
					newModel = false;
					Model model = fileLoader.getModel(newModelPath);
					SModel sModel = { newModelPath.substr(0, newModelPath.find(".")), model };
					sModel.shader = shaders[0];
					simpleModels.push_back(sModel);
					newModelPath.clear();
				}

				ImGui::End();
			}
		}
	}

	void Renderer::sceneObjects(std::vector<SModel> models)
	{
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
	}

	void Renderer::modelParameters(GLFWwindow* m_pWindow, std::vector<SModel> models)
	{
		if (ImGui::Begin("Parameters"))
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
					if (ImGui::Button("Rename"))
					{
						ImGui::InputText("", const_cast<char*>(newName.c_str()), 64);
						if (glfwGetKey(m_pWindow, GLFW_KEY_ENTER))
						{
							models[i].name = newName;
							newName.clear();
						}
					}
					if (ImGui::Selectable(std::string("shader: " + models[i].shader.name).c_str(), true))
					{
						for (int j = 0; j < shaders.size(); j++)
						{
							ImGui::Selectable(shaders[j].name.c_str(), &shaders[j].selected);
							if (shaders[j].selected)
							{
								models[j].shader = shaders[j];
								shaders[j].selected = false;
							}
							if (ImGui::Button("new shader"))
								newShader = true;
						}
					}
				}
			}
			ImGui::End();
		}
	}

	void Renderer::addNewShader(GLFWwindow* m_pWindow)
	{
		if (newShader)
		{
			if (ImGui::Begin("New shader"))
			{
				ImGui::InputText("", const_cast<char*>(newShaderPath.c_str()), 64);

				if ((ImGui::Button("Add") || glfwGetKey(m_pWindow, GLFW_KEY_ENTER)) && !newShaderPath.empty())
				{
					newShader = false;
					ShaderProgram shaderProgram = fileLoader.getShader(newShaderPath);
					SShader shader;
					shader.name = newShaderPath;
					shader.shader = shaderProgram;
					shader.shaderProgram = shaderProgram.getShaderProgram();
					shaders.push_back(shader);
					newShaderPath.clear();
				}

				ImGui::End();
			}
		}
	}

	void Renderer::selectSkybox(std::vector<SSkybox> skyboxes)
	{
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
	}

	void Renderer::askCloseWindow(GLFWwindow* m_pWindow)
	{
		if (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(m_pWindow, true);
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