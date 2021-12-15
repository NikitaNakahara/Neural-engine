#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <string>

namespace Graphic
{
	class ShaderProgram
	{
	public:
		ShaderProgram();
		ShaderProgram(const char* vertexPath, const char* fragmentPath);
		~ShaderProgram();

		GLuint getShaderProgram() { return shaderProgram; }

        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setVec2(const std::string& name, float x, float y) const;
        void setVec3(const std::string& name, float x, float y, float z) const;
        void setVec4(const std::string& name, float x, float y, float z, float w) const;
        void setMat2(const std::string& name, const glm::mat2& mat) const;
        void setMat3(const std::string& name, const glm::mat3& mat) const;
        void setMat4(const std::string& name, const glm::mat4& mat) const;

	private:
		GLuint shaderProgram;
	};
}