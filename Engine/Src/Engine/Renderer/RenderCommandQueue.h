#pragma once

#include "eepch.h"

namespace ee :: re
{
	
	class RenderCommandQueue
	{
		
	private:

		euchar* m_CommandBuffer;
		euchar* m_CommandBufferPtr;

		euint m_RenderCommandCount = 0;

	public:

		using RenderCommand = std::function<euint(void*)>;
		typedef euint(*RenderCommandFn)(void*);

		RenderCommandQueue();
		~RenderCommandQueue();

		void Submit(const RenderCommand& command);
		void SubmitCommand(RenderCommandFn fn, void* params, euint size);

		void Execute();

	};

}