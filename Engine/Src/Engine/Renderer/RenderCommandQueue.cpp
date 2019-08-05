#include "eepch.h"
#include "RenderCommandQueue.h"

#define EE_RENDER_TRACE(...) EE_CORE_TRACE(__VA_ARGS__)

namespace ee::re
{
	RenderCommandQueue::RenderCommandQueue()
	{
		// A 10 mb command buffer.
		m_CommandBuffer = new euchar[10 * 1024 * 1024];
		m_CommandBufferPtr = m_CommandBuffer;
		memset(m_CommandBuffer, 0, 10 * 1024 * 1024);
	}

	RenderCommandQueue::~RenderCommandQueue()
	{
		delete[] m_CommandBuffer;
	}

	void RenderCommandQueue::Submit(const RenderCommand& command)
	{
		auto ptr = m_CommandBuffer;

		memcpy(m_CommandBuffer, &command, sizeof(RenderCommand));
		m_CommandBufferPtr += sizeof(RenderCommand);
		m_RenderCommandCount++;
	}

	void RenderCommandQueue::SubmitCommand(RenderCommandFn fn, void* params, euint size)
	{

		byte*& buffer = m_CommandBufferPtr;
		memcpy(buffer, &fn, sizeof(RenderCommandFn));
		buffer += sizeof(RenderCommandFn);
		memcpy(buffer, params, size);
		buffer += size;

		auto total_size = sizeof(RenderCommandFn) + size;
		auto total_padding = total_size % 16;
		buffer += total_padding;

		m_RenderCommandCount++;

	}

	void RenderCommandQueue::Execute()
	{
		EE_RENDER_TRACE("RenderCommandQueue::Execute -- {0} commands, {1} bytes", m_RenderCommandCount, (m_CommandBufferPtr - m_CommandBuffer));

		byte* buffer = m_CommandBuffer;

		for (int i = 0; i < m_RenderCommandCount; i++)
		{
			RenderCommandFn fn = *reinterpret_cast<RenderCommandFn*>(buffer);
			buffer += sizeof(RenderCommandFn);
			buffer += (*fn)(buffer);

			auto padding = reinterpret_cast<int>(buffer) % 16;
			buffer += padding;
		}

		m_CommandBufferPtr = m_CommandBuffer;
		m_RenderCommandCount = 0;

	}
}
