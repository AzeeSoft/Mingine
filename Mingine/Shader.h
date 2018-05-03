#pragma once
#include <string>
#include <GL/glew.h>

namespace mingine
{
	class Shader
	{
	public:
		Shader(bool init);
		~Shader();

		void init();
		void attachVertexShader(std::string filename) const;
		void attachFragmentShader(std::string filename) const;
		void attachShader(GLenum shaderType, std::string filename) const;
		void use();

		unsigned int getProgramId() const;

	private:
		bool linked = false;
		unsigned int shaderProgramId;
		
		void linkShaders();
	};
}

