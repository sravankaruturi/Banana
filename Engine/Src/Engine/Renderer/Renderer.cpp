#include "eepch.h"
#include "Renderer.h"

namespace ee::re
{

	Renderer* Renderer::s_Instance = new Renderer();
	RendererAPIType RendererAPI::s_CurrentRendererAPI = RendererAPIType::OpenGL;

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
		EE_RENDER({ RendererAPI::Init(); });
	}

	void Renderer::DrawIndexed(euint count)
	{
		EE_RENDER_1(count, { RendererAPI::DrawIndexed(count); });
	}

	void ::ee::re::Renderer::WaitAndRender()
	{
		s_Instance->m_CommandQueue.Execute();
	}
}
