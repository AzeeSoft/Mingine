#include "Shader.h"
#include "OpenGLHelper.h"
#include <iostream>
#include <SDL.h>


namespace mingine
{
	Shader::Shader(bool init)
	{
		if(init)
		{
			this->init();
		}
	}

	Shader::~Shader()
	{
	}

	void Shader::init()
	{
		shaderProgramId = glCreateProgram();
	}

	void Shader::attachVertexShader(std::string filename) const
	{
		attachShader(GL_VERTEX_SHADER, filename);
	}

	void Shader::attachFragmentShader(std::string filename) const
	{
		attachShader(GL_FRAGMENT_SHADER, filename);
	}

	void Shader::attachShader(GLenum shaderType, std::string filename) const
	{
		unsigned int shaderID;
		if (OpenGLHelper::compileShader(shaderType, filename, shaderID))
		{
			glAttachShader(shaderProgramId, shaderID);
		}

		glDeleteShader(shaderID);
	}

	void Shader::use()
	{
		if (!linked)
			linkShaders();

		glUseProgram(shaderProgramId);
	}

	unsigned int Shader::getProgramId() const
	{
		return shaderProgramId;
	}

	void Shader::linkShaders()
	{
		if (linked)
			return;

		glLinkProgram(shaderProgramId);

		int success;
		char infoLog[512];
		glGetProgramiv(shaderProgramId, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgramId, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM_ERROR\n" << infoLog << std::endl;
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Shader linking Error", infoLog, nullptr);
		}

		linked = true;
	}
}

