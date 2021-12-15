#pragma once

#include <glm/glm.hpp>

#include "../shader/ShaderProgram.hpp"

#include <string>
#include <vector>

namespace Graphic
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
	};

	struct Texture
	{
		unsigned int id;
		std::string type;
		std::string path;
	};

	class Mesh
	{
	public:
		std::vector<Vertex>			vertices;
		std::vector<unsigned int>	indices;
		std::vector<Texture>		textures;
		unsigned int VAO;

		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
		~Mesh(){}

		void Draw(ShaderProgram shaderProgram);

	private:
		unsigned int VBO, EBO;
		void setupMesh();
	};
}