#pragma once

#include <string>

namespace Graphic
{
	class ShaderReader
	{
	public:
		ShaderReader(const char* vertexName, const char* fragmentName);
		~ShaderReader();

		const char* getVertexShader() { return vertexCode.c_str(); }
		const char* getFragmentShader() { return fragmentCode.c_str(); }
	private:
		std::string vertexCode;
		std::string fragmentCode;
	};
}