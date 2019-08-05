#include "eepch.h"
#include "RenderCommand.h"

#include "RendererAPI.h"

namespace ee::re
{
	euint RenderCommand::Clear(void* datablock)
	{
		auto data = reinterpret_cast<float*>(datablock);

		float r = *data++;
		float g = *data++;
		float b = *data++;
		float a = *data++;

		RendererAPI::Clear(r, g, b, a);

		return sizeof(float) * 4;

	}
}
