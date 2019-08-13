#pragma once

#include "Engine/Renderer/VertexBuffer.h"

namespace ee::re
{
	class OpenGLVertexBuffer : public VertexBuffer
	{

	private:
		RendererID m_RendererID;
		euint m_Size;

	public:
		OpenGLVertexBuffer(euint size);

		virtual ~OpenGLVertexBuffer();

		virtual void SetData(void* buffer, euint size, euint offset = 0) override;
		virtual void Bind() const override;

		virtual void UnBind() const;

		[[nodiscard]] virtual euint GetSize() const override { return m_Size; }
		[[nodiscard]] virtual RendererID GetRendererID() const override { return m_RendererID; }

	};

}