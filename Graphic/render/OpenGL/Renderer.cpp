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

	void Renderer::on_update(GLFWwindow* m_pWindow, ShaderProgram shaders[], Model models[], GLuint textures[], int width, int height)
	{
		glm::vec3 windows[] = {
			glm::vec3(0.f, 0.f, 3.f),
			glm::vec3(0.5f, 0.5f, 2.f)
		};

		ShaderProgram object_shader_program = shaders[0];
		GLint objectShaderProgram = object_shader_program.getShaderProgram();

		bool cursorVisible = false;
		bool cursor = true;
		bool firstFrame = true;
		glfwSetInputMode(m_pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		LOG_INFO("Program was started");
		while (!glfwWindowShouldClose(m_pWindow))
		{
			if (glfwGetKey(m_pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
				glfwSetWindowShouldClose(m_pWindow, true);

			
			camera.cameraMove(m_pWindow);

			glm::vec3 cameraPos = camera.getCameraPos();
			glm::vec3 cameraFront = camera.getCameraFront();

			projection = glm::perspective(glm::radians(45.0f), static_cast<float>(width) / static_cast<float>(height), 0.1f, 100.0f);

			glClearColor(m_backgroundColor[0], m_backgroundColor[1], m_backgroundColor[2], m_backgroundColor[3]);
			model = glm::mat4(1.0f);

			view = camera.getView();
			glBindFramebuffer(GL_FRAMEBUFFER, FBO);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);

			skybox.drawSkybox(view, projection);

			glDepthFunc(GL_LESS);

			glUseProgram(objectShaderProgram);
			glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.texture);
			object_shader_program.setMat4("view", view);
			object_shader_program.setMat4("projection", projection);
			object_shader_program.setVec3("lightPos", lightPos.x, lightPos.y, lightPos.z);
			object_shader_program.setVec3("viewDir", cameraFront.x, cameraFront.y, cameraFront.z);
			object_shader_program.setVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
			object_shader_program.setMat4("model", model);
			models[0].Draw(object_shader_program);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glClear(GL_COLOR_BUFFER_BIT);

			FBOrender.useFramebuffer();
			glBindVertexArray(quadVAO);
			glDisable(GL_DEPTH_TEST);
			FBOrender.useTexture();
			glDrawArrays(GL_TRIANGLES, 0, 6);

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
			return -1;
		}

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
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

		stbi_set_flip_vertically_on_load(true);
		
		ShaderProgram object_shader = ShaderProgram("..\\..\\res\\shaders\\object.vs", "..\\..\\res\\shaders\\object.fs");
		ShaderProgram shaders[] = { object_shader };
		Model backpack = Model("../../res/models/backpack/backpack.obj");
		Model models[] = { backpack };



		MTexture texture1("..\\..\\res\\textures\\texture.png", GL_TEXTURE_2D, GL_RGBA);
		GLuint textures[] = { texture1.getTexture() };

		FBOrender.framebufferInit(width, height);
		FBO = FBOrender.getFBO();
		quadVAO = FBOrender.getVAO();

		on_update(m_pWindow, shaders, models, textures, width, height);

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