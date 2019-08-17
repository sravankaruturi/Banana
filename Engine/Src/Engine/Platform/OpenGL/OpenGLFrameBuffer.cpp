#include "eepch.h"
#include "OpenGLFrameBuffer.h"

#include "Engine/Renderer/Renderer.h"
#include <glad/glad.h>

namespace ee::re
{
	OpenGLFrameBuffer::OpenGLFrameBuffer(uint32_t width, uint32_t height, FrameBufferFormat format)
		:m_Width(width), m_Height(height), m_Format(format)
	{
		Resize(width, height);
	}

	OpenGLFrameBuffer::~OpenGLFrameBuffer()
	{
		EE_RENDER_S(
			{
				glDeleteFramebuffers(1, &self->m_RendererId);
			}
		);
	}

	void OpenGLFrameBuffer::Resize(uint32_t width, uint32_t height)
	{

		if ( m_Width == width && m_Height == height)
		{
			return;
		}

		m_Width = width;
		m_Height = height;

		EE_RENDER_S({

			if (self->m_RendererId)
			{
				glDeleteFramebuffers(1, &self->m_RendererId);
				glDeleteTextures(1, &self->m_ColourAttachment);
				glDeleteTextures(1, &self->m_DepthAttachment);
			}

			glGenFramebuffers(1, &self->m_RendererId);
			glBindFramebuffer(GL_FRAMEBUFFER, self->m_RendererId);

			glGenTextures(1, &self->m_ColourAttachment);
			glBindTexture(GL_TEXTURE_2D, self->m_ColourAttachment);

			// TODO: Create a custom texture Object based on format here.
			if (self->m_Format == FrameBufferFormat::RGBA16F)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, self->m_Width, self->m_Height, 0, GL_RGBA, GL_FLOAT, nullptr);
			}
			else if (self->m_Format == FrameBufferFormat::RGBA8)
			{
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, self->m_Width, self->m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

			}

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, self->m_ColourAttachment, 0);

			glGenTextures(1, &self->m_DepthAttachment);
			glBindTexture(GL_TEXTURE_2D, self->m_DepthAttachment);
			glTexImage2D(
				GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, self->m_Width, self->m_Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL
			);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, self->m_DepthAttachment, 0);
			if ( glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
			{
				EE_CORE_ERROR("FrameBuffer is incomplete!");
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			});
		
	}

	void OpenGLFrameBuffer::Bind() const
	{

		EE_RENDER_S({
			glBindFramebuffer(GL_FRAMEBUFFER, self->m_RendererId);
			glViewport(0, 0, self->m_Width, self->m_Height);
			});
		
	}

	void OpenGLFrameBuffer::Unbind() const
	{
		EE_RENDER_S({
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			});
	}

	void OpenGLFrameBuffer::BindTexture(uint32_t slot) const
	{
		EE_RENDER_S1(slot, {
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, self->m_ColourAttachment);
			});
	}
}
