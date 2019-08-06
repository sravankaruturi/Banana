#pragma once

#include "eepch.h"

namespace ee :: re
{
	
	class RenderCommandQueue
	{
		
	private:

		euchar* m_CommandBuffer;
		euchar* m_CommandBufferPtr;

		euint m_CommandCount = 0;

	public:

		using RenderCommand = std::function<euint(void*)>;
		typedef void(*RenderCommandFn)(void*);

		RenderCommandQueue();
		~RenderCommandQueue();

		void* Allocate(RenderCommandFn func, euint size);

		void Execute();

	};

}