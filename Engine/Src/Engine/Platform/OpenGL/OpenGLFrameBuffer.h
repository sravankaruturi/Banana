#pragma once
#include "Engine/Renderer/FrameBuffer.h"

namespace ee::re
{
	class OpenGLFrameBuffer : public FrameBuffer
	{

	private:
		RendererID m_RendererId = 0;
		uint32_t m_Width, m_Height;
		FrameBufferFormat m_Format;

		RendererID m_DepthAttachment, m_ColourAttachment;
		
	public:

		OpenGLFrameBuffer(uint32_t width, uint32_t height, FrameBufferFormat format);
		virtual ~OpenGLFrameBuffer();
		
		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void BindTexture(uint32_t slot = 0) const override;

		[[nodiscard]] RendererID GetRendererID() const override { return m_RendererId; }
		RendererID GetColourAttachmentRendererID() const override { return m_ColourAttachment; }
		RendererID GetDepthAttachmentRendererID() const override { return m_DepthAttachment; }

		virtual uint32_t GetWidth() const { return m_Width; }
		virtual uint32_t GetHeight() const { return m_Height; }
		virtual FrameBufferFormat GetFormat() const { return m_Format; }

	};

}