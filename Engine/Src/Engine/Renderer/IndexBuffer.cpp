#include "eepch.h"
#include "Engine/Platform/OpenGL/OpenGLIndexBuffer.h"

namespace ee::re
{
	
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