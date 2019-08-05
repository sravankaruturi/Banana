#pragma once

#include "RenderCommandQueue.h"
#include "RendererAPI.h"

namespace ee
{
	namespace re // re is rendering engine.
	{
		
		class Renderer
		{
			
		private:

			static Renderer* s_Instance;
			RenderCommandQueue m_CommandQueue;

		public:

			static void Clear();
			static void Clear(float r, float g, float b, float a = 1.f);
			static void SetClearColour(float r, float g, float b, float a);

			static void ClearMagenta();

			void Init();

			static void Submit(const std::function<void()>& command)
			{
				
			}

			void WaitAndRender();

			inline static Renderer& GetInstance() { return *s_Instance; }

		};

#define EE_RENDER(x) ::ee::re::Renderer::Submit([=](){RendererAPI::x;})

	}
}