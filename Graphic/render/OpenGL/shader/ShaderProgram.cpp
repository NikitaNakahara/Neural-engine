#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>

#include "ShaderProgram.hpp"
#include "ShaderReader.hpp"
#include "../../../Log.hpp"

namespace Graphic
{
    ShaderProgram::ShaderProgram()
    {}

	ShaderProgram::ShaderProgram(const char * vertexPath, const char* fragmentPath)
	{
		ShaderReader shader = ShaderReader(vertexPath, fragmentPath);
		const char* vertexShaderSource = shader.getVertexShader();
		const char* fragmentShaderSource = shader.getFragmentShader();

		GLint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
		glCompileShader(vertexShader);
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			LOG_CRITICAL("Filed compile vertex shader: {0}", infoLog);
		}

		GLint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			LOG_CRITICAL("Filed compile fragment shader: {0}", infoLog);
		}

		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			LOG_CRITICAL("Filed linking shader program: {0}", infoLog);
		}

		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	ShaderProgram::~ShaderProgram()
	{
	}

    void ShaderProgram::setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), (int)value);
    }
    // ------------------------------------------------------------------------
    void ShaderProgram::setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(shaderProgram, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void ShaderProgram::setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(shaderProgram, name.c_str()), value);
    }
    // ------------------------------------------------------------------------
    void ShaderProgram::setVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(shaderProgram, name.c_str()), x, y);
    }
    // ------------------------------------------------------------------------
    void ShaderProgram::setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z);
    }
    // ------------------------------------------------------------------------
    void ShaderProgram::setVec4(const std::string& name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(shaderProgram, name.c_str()), x, y, z, w);
    }
    // ------------------------------------------------------------------------
    void ShaderProgram::setMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void ShaderProgram::setMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    // ------------------------------------------------------------------------
    void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
}