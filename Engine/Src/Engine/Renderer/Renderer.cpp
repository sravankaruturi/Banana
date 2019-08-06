﻿#include "eepch.h"
#include "Renderer.h"

namespace ee::re
{

	Renderer* Renderer::s_Instance = new Renderer();

	void ::ee::re::Renderer::Clear()
	{
	}

	void ::ee::re::Renderer::Clear(float r, float g, float b, float a)
	{
		EE_RENDER_4(r, g, b, a, {
			RendererAPI::Clear(r, g, b, a);
		});
	}

	void ::ee::re::Renderer::SetClearColour(float r, float g, float b, float a)
	{
	}

	void ::ee::re::Renderer::ClearMagenta()
	{
		Clear(0.8f, 0.1f, 0.8f);
	}

	void ::ee::re::Renderer::Init()
	{
	}

	void ::ee::re::Renderer::WaitAndRender()
	{
		s_Instance->m_CommandQueue.Execute();
	}
}
