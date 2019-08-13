#pragma once

#include "Engine/Renderer/IndexBuffer.h"

namespace ee::re
{
	class OpenGLIndexBuffer : public IndexBuffer
	{

	private:
		RendererID m_RendererID;
		euint m_Size;

	public:
		OpenGLIndexBuffer(euint size);
		virtual ~OpenGLIndexBuffer();


		virtual void SetData(void* buffer, euint size, euint offset) override;
		virtual void Bind() const override;

		[[nodiscard]] virtual euint GetSize() const override { return m_Size; };
		[[nodiscard]] virtual RendererID GetRendererID() const override { return m_RendererID; };
		
		virtual void UnBind() const override;
	};

}
