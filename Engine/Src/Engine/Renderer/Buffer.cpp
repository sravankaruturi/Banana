#include "eepch.h"

#include "Engine/Platform/OpenGL/OpenGLBuffer.h"

namespace ee::re
{

	VertexBuffer* VertexBuffer::Create(euint size /* = 0 */)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None:
			return nullptr;
		case RendererAPIType::OpenGL:
			return new OpenGLVertexBuffer(size);
		}
	}

	IndexBuffer* IndexBuffer::Create(euint size)
	{
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None:
			return nullptr;
		case RendererAPIType::OpenGL:
			return new OpenGLIndexBuffer(size);
		}
	}

}
