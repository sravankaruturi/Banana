#pragma once

#include "Engine/Core/Core.h"

namespace ee::re
{

	enum class TextureFormat
	{
		None	= 0,
		RGB		= 1,
		RGBA	= 2,
	};

	class Texture
	{
	public:
		virtual ~Texture() = default;
	};

	class Texture2D : public Texture
	{
		
	public:
		static Texture2D* Create(TextureFormat format, euint width, euint height);

		virtual TextureFormat GetFormat() const = 0;
		virtual euint GetWidth() const = 0;
		virtual euint GetHeight() const = 0;

	};

}
