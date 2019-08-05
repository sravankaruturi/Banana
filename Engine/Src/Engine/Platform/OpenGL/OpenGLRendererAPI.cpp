#include "eepch.h"
#include "Engine/Renderer/RendererAPI.h"
#include "glad/glad.h"

namespace ee::re
{

	void RendererAPI::Clear(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void RendererAPI::SetClearColour(float r, float g, float b, float a)
	{
		glClearColor(r, g, b, a);
	}


}
