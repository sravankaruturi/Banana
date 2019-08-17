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

		unsigned char* m_ImageData;
		std::string m_FilePath;

	public:
		
		OpenGLTexture2D(TextureFormat format, euint width, euint height);
		OpenGLTexture2D(const std::string& path, bool srgb);

		virtual ~OpenGLTexture2D();
	
		virtual void Bind(euint slot) const override;

		[[nodiscard]] virtual TextureFormat GetFormat() const override { return m_Format; };
		[[nodiscard]] virtual euint GetWidth() const override { return m_Width; };
		[[nodiscard]] virtual euint GetHeight() const override { return m_Height; };
		[[nodiscard]] virtual const std::string& GetPath() const override { return m_FilePath; };
		[[nodiscard]] virtual RendererID GetRendererID() const override { return m_RendererID; };
	};

	class OpenGLTextureCube : public TextureCube
	{

	private:
		RendererID m_RendererID;
		TextureFormat m_Format;
		euint m_Width, m_Height;

		unsigned char* m_ImageData;
		std::string m_FilePath;
		
	public :

		OpenGLTextureCube(const std::string& path);
		
		virtual void Bind(unsigned slot) const override;
		
		virtual ~OpenGLTextureCube() override;

		[[nodiscard]] virtual RendererID GetRendererID() const override { return m_RendererID; };
		[[nodiscard]] virtual TextureFormat GetFormat() const override { return m_Format; };
		[[nodiscard]] virtual unsigned GetWidth() const override { return m_Width; };
		[[nodiscard]] virtual unsigned GetHeight() const override { return m_Height; };
		[[nodiscard]] virtual const std::string& GetPath() const override { return m_FilePath; };
		
	};

}
