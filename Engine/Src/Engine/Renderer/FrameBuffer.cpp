#include "eepch.h"
#include "FrameBuffer.h"

#include "Engine/Platform/OpenGL/OpenGLFrameBuffer.h"

namespace ee::re
{

	FrameBuffer* FrameBuffer::Create(uint32_t width, uint32_t height, FrameBufferFormat format)
	{
		FrameBuffer* result = nullptr;
		switch (RendererAPI::Current())
		{
		case RendererAPIType::None:
			return nullptr;
		case RendererAPIType::OpenGL:
			result = new OpenGLFrameBuffer(width, height, format);
		}
		FrameBufferPool::GetGlobal()->Add(result);
		return result;
	}

	/*
	 * Frame Buffer Pool
	 */

	FrameBufferPool* FrameBufferPool::s_Instance = new FrameBufferPool;

	FrameBufferPool::FrameBufferPool(uint32_t maxFBs)
	{
		
	}

	FrameBufferPool::~FrameBufferPool()
	{
		
	}

	std::weak_ptr<FrameBuffer> FrameBufferPool::AllocateBuffer()
	{
		return std::weak_ptr<FrameBuffer>();
	}

	void FrameBufferPool::Add(FrameBuffer* framebuffer)
	{
		m_Pool.push_back(framebuffer);
	}
}
