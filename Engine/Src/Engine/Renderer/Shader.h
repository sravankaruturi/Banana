#pragma once

#include "Engine/Core/Core.h"
#include "Engine/Renderer/Renderer.h"

namespace ee::re
{

	struct ShaderUniform
	{
		
	};

	struct ShaderUniformCollection
	{
		
	};

	class Shader
	{

	public:
		virtual void Bind() = 0;

		static Shader* Create(const std::string& filePath);

	};
}
