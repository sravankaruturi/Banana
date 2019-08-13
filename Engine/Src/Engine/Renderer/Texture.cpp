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
}
