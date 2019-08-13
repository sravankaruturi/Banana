#pragma once

#include "Engine/Renderer/RendererAPI.h"
#include "Engine/Renderer/Texture.h"

namespace ee::re
{
	
	class OpenGLTexture2D : public Texture2D
	{

	private:
		RendererID m_RendererID;
		TextureFormat m_Format;
		euint m_Width, m_Height;

	public:
		OpenGLTexture2D(TextureFormat format, euint width, euint height);
		~OpenGLTexture2D();

		[[nodiscard]] virtual TextureFormat GetFormat() const override { return m_Format; };
		[[nodiscard]] virtual euint GetWidth() const override { return m_Width; };
		[[nodiscard]] virtual euint GetHeight() const override { return m_Height; };
	};

}
