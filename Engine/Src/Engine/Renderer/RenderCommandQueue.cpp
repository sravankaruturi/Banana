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

	void* RenderCommandQueue::Allocate(RenderCommandFn fn, euint size)
	{

		// TODO: Alignment

		// We reinterpret our char* into a RenderCommandFn struct and assign the command function here.
		*reinterpret_cast<RenderCommandFn*>(m_CommandBufferPtr) = fn;

		// And update it by the size.
		m_CommandBufferPtr += sizeof(RenderCommandFn);

		// Add the size input at the end, and update the pointer to the next available pointer.
		*reinterpret_cast<int*>(m_CommandBufferPtr) = size;
		m_CommandBufferPtr += sizeof(euint);
		
		void* memory = m_CommandBufferPtr;
		m_CommandBufferPtr += size;

		m_CommandCount++;
		return memory;

	}

	void RenderCommandQueue::Execute()
	{
		EE_RENDER_TRACE("RenderCommandQueue::Execute -- {0} commands, {1} bytes", m_CommandCount, (m_CommandBufferPtr - m_CommandBuffer));

		byte* buffer = m_CommandBuffer;

		for (euint i = 0; i < m_CommandCount; i++)
		{
			RenderCommandFn function = *reinterpret_cast<RenderCommandFn*>(buffer);
			buffer += sizeof(RenderCommandFn);

			euint size = *reinterpret_cast<euint*>(buffer);
			buffer += sizeof(euint);
			function(buffer);
			buffer += size;
		}

		m_CommandBufferPtr = m_CommandBuffer;
		m_CommandCount = 0;

	}
}
