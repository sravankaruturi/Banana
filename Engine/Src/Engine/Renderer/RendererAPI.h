#pragma once
#include <stdint.h>

namespace ee :: re
{

	using RendererID = uint32_t;

	enum class RendererAPIType
	{
		None,
		OpenGL
	};

	struct RenderAPICapabilities
	{
		std::string Vendor;
		std::string Renderer;
		std::string Version;

		int MaxSamples;
		float MaxAnisotropy;
	};
	
	class RendererAPI
	{
		
	private:
		static RendererAPIType s_CurrentRendererAPI;

	public:
		static void Clear(float r, float g, float b, float a);
		static void SetClearColour(float r, float g, float b, float a);

		static void DrawIndexed(euint count, bool depthTest = true);

		static RenderAPICapabilities& GetCapabilities()
		{
			static RenderAPICapabilities capabilities;
			return capabilities;
		}
		
		static void Init();
		static void Shutdown();

		static RendererAPIType Current() { return s_CurrentRendererAPI; }

	};

}
