#include "eepch.h"
#include "Engine/Renderer/RendererAPI.h"

#include "OpenGL.h"

namespace ee::re
{

	void RendererAPI::Init()
	{
		unsigned int vao;
		PE_GL(glGenVertexArrays(1, &vao));
		PE_GL(glBindVertexArray(vao));
	}

	void RendererAPI::Shutdown()
	{
		
	}

	void RendererAPI::Clear(float r, float g, float b, float a)
	{
		PE_GL(glClearColor(r, g, b, a));
		PE_GL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
	}

	void RendererAPI::SetClearColour(float r, float g, float b, float a)
	{
		PE_GL(glClearColor(r, g, b, a));
	}

	void RendererAPI::DrawIndexed(unsigned int count)
	{
		PE_GL(glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr));
	}

}
