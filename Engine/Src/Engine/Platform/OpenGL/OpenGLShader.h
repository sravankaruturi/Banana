#pragma once

#include "Engine/Renderer/Shader.h"
#include "OpenGL.h"

namespace ee::re
{

	class OpenGLShader : public Shader
	{
		
	private:
		RendererID m_RendererID;
		std::string m_ShaderSource;

		void ReadShaderFromFile(const std::string& filePath);
		void CompileAndUploadShader();

	public:

		OpenGLShader(const std::string& filePath);
		virtual void Bind() override;

		static GLenum ShaderTypeFromString(const std::string& type);

	};
	
}