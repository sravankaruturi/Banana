#include "eepch.h"
#include "Shader.h"

#include "Engine/Platform/OpenGL/OpenGLShader.h"

namespace ee::re
{
	
	Shader* Shader::Create(const std::string& filePath)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None:
			return nullptr;
		case RendererAPIType::OpenGL:
			return new OpenGLShader(filePath);
		}
		return nullptr;
	}

}