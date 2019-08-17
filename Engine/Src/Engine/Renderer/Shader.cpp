#include "eepch.h"
#include "Shader.h"

#include "Engine/Platform/OpenGL/OpenGLShader.h"

namespace ee::re
{

	std::vector<Shader*> Shader::s_AllShaders;
	
	Shader* Shader::Create(const std::string& filePath)
	{

		Shader* result = nullptr;
		
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None:
			return nullptr;
		case RendererAPIType::OpenGL:
			result = new OpenGLShader(filePath);
		}

		s_AllShaders.push_back(result);
		return result;
	}

}