#include "eepch.h"
#include "Texture.h"

#include "Engine/Renderer/RendererAPI.h"
#include "Engine/Platform/OpenGL/OpenGLTexture.h"

namespace ee::re
{
	Texture2D* Texture2D::Create(TextureFormat format, euint width, euint height)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None:
			return nullptr;
		case RendererAPIType::OpenGL:
			return new OpenGLTexture2D(format, width, height);
		}
	}


	Texture2D* Texture2D::Create(const std::string& path, bool srgb)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None:
			return nullptr;
		case RendererAPIType::OpenGL:
			return new OpenGLTexture2D(path, srgb);
		}
	}

	TextureCube* TextureCube::Create(const std::string& path)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None:
			return nullptr;
		case RendererAPIType::OpenGL:
			return new OpenGLTextureCube(path);
		}
	}
}
