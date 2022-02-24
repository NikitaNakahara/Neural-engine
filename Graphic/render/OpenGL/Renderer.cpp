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
	Skybox skybox = Skybox();

	Renderer::Renderer()
	{}

	Renderer::~Renderer()
	{
		shutdown();
	}

	void Renderer::on_update(GLFWwindow* m_pWindow, int width, int height)
	{
		glm::vec3 windows[] = {
			glm::vec3(0.f, 0.f, 3.f),
			glm::vec3(0.5f, 0.5f, 2.f)
		};

		bool cursorVisible = false;
		bool cursor = true;
		bool firstFrame = true;
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		LOG_INFO("Program was started");
		while (!glfwWindowShouldClose(m_pWindow))
		{
			if (glfwGetKey(m_pWindow, GLFW_KEY_END) == GLFW_PRESS)
				glfwSetWindowShouldClose(m_pWindow, true);

			
			camera.cameraMove(m_pWindow);

			glm::vec3 cameraPos = camera.getCameraPos();
			glm::vec3 cameraFront = camera.getCameraFront();

			projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

			glClearColor(m_backgroundColor[0], m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3]);

			view = camera.getView();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);

			skybox.drawSkybox(view, projection);

			for (int i = 0; i < models.size(); i++)
			{
				if (models[i].draw)
				{
					glUseProgram(models[i].shader_program);
					models[i].shader.setMat4("view", view);
					models[i].shader.setMat4("projection", projection);
					models[i].shader.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
					models[i].shader.setVec3("viewDir", cameraFront.x, cameraFront.y, cameraFront.z);
					models[i].shader.setVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
					models[i].model_mat = glm::mat4(1.0f);
					models[i].model_mat = glm::translate(models[i].model_mat, glm::vec3(models[i].position[0], models[i].position[1], models[i].position[2]));
					models[i].shader.setMat4("model", models[i].model_mat);
					models[i].model.Draw(models[i].shader);
				}
			}
			

			ImGuiIO& io = ImGui::GetIO();
			io.DisplaySize.x = static_cast<float>(width);
			io.DisplaySize.y = static_cast<float>(height);

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			if (ImGui::Begin("Scene models"))
			{
				for (int i = 0; i < models.size(); i++)
				{
					ImGui::Selectable(models[i].name.c_str(), &models[i].selected);
					if (models[i].selected)
					{
						for (int j = 0; j < models.size(); j++)
						{
							if (i != j)
								models[j].selected = false;
						}
					}
				}
				ImGui::End();
			}

			if (ImGui::Begin("Parameters"))
			{
				for (int i = 0; i < models.size(); i++)
				{
					if (models[i].selected)
					{
						ImGui::Selectable(models[i].name.c_str(), true);
						ImGui::SliderFloat3("pos", models[i].position, -10.0f, 10.0f);
						ImGui::Checkbox("Draw", &models[i].draw);
					}
				}

				ImGui::End();
			}

			if (glfwGetKey(m_pWindow, GLFW_KEY_A) && glfwGetKey(m_pWindow, GLFW_KEY_LEFT_CONTROL))
				GUIModels = true;

			if (GUIModels)
			{
				if (ImGui::Begin("Models"))
				{
					for (int i = 0; i < allModels.size(); i++)
					{
						if (!allModels[i].selected)
						{
							if (ImGui::Selectable(allModels[i].name.c_str()) || glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE))
							{
								GUIModels = false;
								models.push_back(allModels[i]);
							}
						}
					}

					ImGui::End();
				}
			}

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
			return -1;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui_ImplOpenGL3_Init();
		ImGui_ImplGlfw_InitForOpenGL(m_pWindow, true);

		stbi_set_flip_vertically_on_load(true);
		
		ShaderProgram object_shader = ShaderProgram("..\\..\\res\\shaders\\object.vs", "..\\..\\res\\shaders\\object.fs");
		Model backpack = Model("../../res/models/backpack/backpack.obj");
		SModel backpack_struct = { object_shader, object_shader.getShaderProgram(), backpack, glm::mat4(1.0f), "backpack" };
		allModels.push_back(backpack_struct);


		MTexture texture1("..\\..\\res\\textures\\texture.png", GL_TEXTURE_2D, GL_RGBA);
		GLuint textures[] = { texture1.getTexture() };

		on_update(m_pWindow, width, height);

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