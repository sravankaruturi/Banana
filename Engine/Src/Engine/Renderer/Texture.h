#pragma once

#include "Engine/Core/Core.h"
#include "RendererAPI.h"

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

		virtual RendererID GetRendererID() const = 0;
	};

	class Texture2D : public Texture
	{
		
	public:
		static Texture2D* Create(TextureFormat format, euint width, euint height);
		static Texture2D* Create(const std::string& path, bool srgb = false);

		virtual void Bind(euint slot = 0) const = 0;

		virtual TextureFormat GetFormat() const = 0;
		virtual euint GetWidth() const = 0;
		virtual euint GetHeight() const = 0;

		virtual const std::string& GetPath() const = 0;

	};

	class TextureCube : public Texture
	{

	public:
		static TextureCube* Create(const std::string& path);

		virtual void Bind(unsigned int slot = 0) const = 0;

		virtual TextureFormat GetFormat() const = 0;
		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		virtual const std::string& GetPath() const = 0;
		
	};

}
