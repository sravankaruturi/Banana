#include "eepch.h"
#include "OpenGLTexture.h"

#include "Engine/Renderer/RendererAPI.h"
#include "Engine/Renderer/Renderer.h"

#include "OpenGL.h"

namespace ee::re
{
	
	static GLenum ToOpenGLTextureFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::None:
			return GL_NONE;
		case TextureFormat::RGB:
			return GL_RGB;
		case TextureFormat::RGBA:
			return GL_RGBA;
		}
		return GL_NONE;
	}


	OpenGLTexture2D::OpenGLTexture2D(TextureFormat format, euint width, euint height)
		: m_Format(format), m_Width(width), m_Height(height)
	{
		auto self = this;
		EE_RENDER_1(self, {
			PE_GL(glGenTextures(1, &self->m_RendererID));
			PE_GL(glBindTexture(GL_TEXTURE_2D, self->m_RendererID));
			PE_GL(glTexImage2D(
				GL_TEXTURE_2D, 0, ToOpenGLTextureFormat(self->m_Format), self->m_Width, self->m_Height, 0, ToOpenGLTextureFormat(self->m_Format), GL_UNSIGNED_BYTE, nullptr
			));
			PE_GL(glBindTexture(GL_TEXTURE_2D, 0));
		});
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		auto self = this;
		EE_RENDER_1(self,
			{
				PE_GL(glDeleteTextures(1, &self->m_RendererID));
			}
		);
	}
}
