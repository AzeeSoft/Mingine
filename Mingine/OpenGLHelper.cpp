#include "OpenGLHelper.h"
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <SDL.h>


namespace mingine
{
	bool OpenGLHelper::compileShader(GLenum shaderType, std::string filename, unsigned int& shaderObjectId)
	{
		shaderObjectId = glCreateShader(shaderType);

		std::ifstream shader_file(filename);
		std::stringstream shader_source_buffer;
		shader_source_buffer << shader_file.rdbuf();
		shader_file.close();

		std::string shaderSrcString = shader_source_buffer.str();
		const char* shader_source = shaderSrcString.c_str();

		glShaderSource(shaderObjectId, 1, &(shader_source), NULL);
		glCompileShader(shaderObjectId);

		int success;
		char infoLog[512];
		glGetShaderiv(shaderObjectId, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(shaderObjectId, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << filename << "::COMPILATION_FAILED\n" << infoLog << std::endl;
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Shader Compile Error", infoLog, nullptr);
		}

		return success;
	}

	OpenGLHelper::OpenGLHelper()
	{
	}


	OpenGLHelper::~OpenGLHelper()
	{
	}
}

