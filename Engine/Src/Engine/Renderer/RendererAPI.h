#pragma once

namespace ee :: re
{

	using RendererID = unsigned int;

	enum class RendererAPIType
	{
		None,
		OpenGL
	};
	
	class RendererAPI
	{
		
	private:
		static RendererAPIType s_CurrentRendererAPI;

	public:
		static void Clear(float r, float g, float b, float a);
		static void SetClearColour(float r, float g, float b, float a);

		static void Init();
		static void Shutdown();

		static void DrawIndexed(unsigned int count);

		static RendererAPIType Current() { return s_CurrentRendererAPI; }

	};

}
