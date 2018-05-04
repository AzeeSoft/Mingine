#pragma once
#include <string>
#include <GL/glew.h>

namespace mingine
{
	class OpenGLHelper
	{
	public:
		static bool compileShader(GLenum shaderType, std::string filename, unsigned int& shaderObjectId);

	private:
		OpenGLHelper();
		~OpenGLHelper();
	};
}

