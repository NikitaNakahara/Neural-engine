#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "skyboxTexture.hpp"
#include "stb_image.h"
#include "../../../Log.hpp"
#include "../shader/ShaderProgram.hpp"
#include <glm/glm.hpp>

#include <vector>
#include <string>

namespace Graphic
{
	Skybox::Skybox()
	{}

	Skybox::~Skybox()
	{}

	GLuint Skybox::initSkybox(std::vector<std::string> faces)
	{
		stbi_set_flip_vertically_on_load(false);
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

		int width, height, nrChannels;


		for (unsigned int i = 0; i < faces.size(); i++)
		{

			unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
			if (data)
			{
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
					0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
				);
				stbi_image_free(data);
			}

			else
			{
				LOG_ERROR("Cubemap tex failed to load at path: {0}", faces[i]);
				stbi_image_free(data);
			}
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return texture;
	}

	void Skybox::drawSkybox(glm::mat4 view, glm::mat4 projection, std::string path)
	{
		if (!loaded)
		{
			std::vector<std::string> faces
			{
				path + "right.jpg",
				path + "left.jpg",
				path + "top.jpg",
				path + "bottom.jpg",
				path + "front.jpg",
				path + "back.jpg"
			};

			GLuint cubeTexture = initSkybox(faces);
			texture = cubeTexture;
			LOG_INFO("skybox texture: {0}", texture);


			float skyboxVertices[] = {
				-1.0f,  1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				-1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f
			};

			skyboxShader = ShaderProgram("..\\..\\res\\shaders\\skybox.vs", "..\\..\\res\\shaders\\skybox.fs");
			
			shader = skyboxShader.getShaderProgram();
			LOG_INFO("skybox shader program: {0}", shader);

			glGenVertexArrays(1, &skyboxVAO);
			LOG_INFO("skybox VAO: {0}", skyboxVAO);
			glGenBuffers(1, &VBO);

			glBindVertexArray(skyboxVAO);

			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
			loaded = true;
		}

		view = glm::mat4(glm::mat3(view));
		glDepthMask(GL_FALSE);
		glDepthFunc(GL_LEQUAL);
		glUseProgram(shader);
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		glBindVertexArray(skyboxVAO);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthMask(GL_TRUE);
	}
}