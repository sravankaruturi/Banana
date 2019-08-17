#pragma once

#include "Engine/Renderer/Renderer.h"

namespace ee::re
{

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer() = default;

		virtual void SetData(void* buffer, euint size, euint offset = 0) = 0;
		virtual void Bind() const = 0;

		virtual euint GetSize() const = 0;
		virtual RendererID GetRendererID() const = 0;

		static VertexBuffer* Create(euint size = 0);
	};

	class IndexBuffer
	{

	public:
		virtual ~IndexBuffer() = default;

		virtual void SetData(void* buffer, euint size, euint offset = 0) = 0;
		virtual void Bind() const = 0;

		virtual euint GetCount() const = 0;
		virtual euint GetSize() const = 0;
		
		virtual RendererID GetRendererID() const = 0;

		static IndexBuffer* Create(euint size = 0);

		virtual void UnBind() const = 0;

	};
	

}
