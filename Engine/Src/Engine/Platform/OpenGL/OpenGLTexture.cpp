﻿#include "eepch.h"
#include "OpenGLTexture.h"

#include "Engine/Renderer/RendererAPI.h"
#include "Engine/Renderer/Renderer.h"

#include "OpenGL.h"
#include <stb_image.h>

namespace ee::re
{

	static int CalculateMipMapCount(int width, int height)
	{
		int levels = 1;
		while( (width | height) >> levels )
		{
			levels++;
		}
		return levels;
	}
	
	static GLenum ToOpenGLTextureFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::None:
			return GL_NONE;
		case TextureFormat::RGB:
			return GL_RGB;
		case TextureFormat::RGBA:
			return GL_RGBA;
		}
		return GL_NONE;
	}


	OpenGLTexture2D::OpenGLTexture2D(TextureFormat format, euint width, euint height)
		: m_Format(format), m_Width(width), m_Height(height)
	{
		auto self = this;
		EE_RENDER_1(self, {

			glGenTextures(1, &self->m_RendererID);
			glBindTexture(GL_TEXTURE_2D, self->m_RendererID);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTextureParameterf(self->m_RendererID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);
			glTexImage2D(
				GL_TEXTURE_2D, 0, ToOpenGLTextureFormat(self->m_Format),
				self->m_Width, self->m_Height, 0, ToOpenGLTextureFormat(self->m_Format), GL_UNSIGNED_BYTE, nullptr
			);

			glGenerateMipmap(GL_TEXTURE_2D);

			glBindTexture(GL_TEXTURE_2D, 0);

		});
	}


	OpenGLTexture2D::OpenGLTexture2D(const std::string& path, bool srgb)
	{
		int width, height, channels;
		EE_CORE_INFO("Loading texture {0}, srgb={1}", path, srgb);
		m_ImageData = stbi_load(path.c_str(), &width, &height, &channels, srgb ? STBI_rgb : STBI_rgb_alpha);

		m_Width = width;
		m_Height = height;
		m_Format = TextureFormat::RGBA;

		EE_RENDER_S1(srgb, {
			// TODO: Consolidate properly
			if (srgb)
			{
				glCreateTextures(GL_TEXTURE_2D, 1, &self->m_RendererID);
				int levels = CalculateMipMapCount(self->m_Width, self->m_Height);
				EE_CORE_INFO("Creating srgb texture width {0} mips", levels);
				glTextureStorage2D(self->m_RendererID, levels, GL_SRGB8, self->m_Width, self->m_Height);
				glTextureParameteri(self->m_RendererID, GL_TEXTURE_MIN_FILTER, levels > 1 ? GL_LINEAR_MIPMAP_LINEAR : GL_LINEAR);
				glTextureParameteri(self->m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glTextureSubImage2D(self->m_RendererID, 0, 0, 0, self->m_Width, self->m_Height, GL_RGB, GL_UNSIGNED_BYTE, self->m_ImageData);
				glGenerateTextureMipmap(self->m_RendererID);
			}
			else
			{
				glGenTextures(1, &self->m_RendererID);
				glBindTexture(GL_TEXTURE_2D, self->m_RendererID);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

				glTexImage2D(GL_TEXTURE_2D, 0, ToOpenGLTextureFormat(self->m_Format), self->m_Width, self->m_Height, 0, srgb ? GL_SRGB8 : ToOpenGLTextureFormat(self->m_Format), GL_UNSIGNED_BYTE, self->m_ImageData);
				glGenerateMipmap(GL_TEXTURE_2D);

				glBindTexture(GL_TEXTURE_2D, 0);
			}
			stbi_image_free(self->m_ImageData);
			});
	}

	OpenGLTexture2D::~OpenGLTexture2D()
	{
		auto self = this;
		EE_RENDER_1(self,
			{
				PE_GL(glDeleteTextures(1, &self->m_RendererID));
			}
		);
	}


	void OpenGLTexture2D::Bind(euint slot) const
	{

		EE_RENDER_S1(slot,
			{
				glBindTextureUnit(slot, self->m_RendererID);
			});
		
	}

	OpenGLTextureCube::OpenGLTextureCube(const std::string& path)
		: m_FilePath(path)
	{

		int width, height, channels;

		stbi_set_flip_vertically_on_load(false);

		m_ImageData = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb);

		m_Width = width;
		m_Height = height;
		m_Format = TextureFormat::RGB;

		euint faceWidth = m_Width / 4;
		euint faceHeight = m_Height / 3;

		EE_CORE_ASSERT(faceWidth == faceHeight, "Non Square Faces!");

		std::array<unsigned char*, 6> faces;
		for (auto & face : faces)
		{
			face = new unsigned char[faceHeight * faceWidth * 3];
		}

		int faceIndex = 0;


		for (size_t i = 0; i < 4; i++)
		{
			for (size_t y = 0; y < faceHeight; y++)
			{
				size_t yOffset = y + faceHeight;
				for (size_t x = 0; x < faceWidth; x++)
				{
					size_t xOffset = x + i * faceWidth;
					faces[faceIndex][(x + y * faceWidth) * 3 + 0] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 0];
					faces[faceIndex][(x + y * faceWidth) * 3 + 1] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 1];
					faces[faceIndex][(x + y * faceWidth) * 3 + 2] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 2];
				}
			}
			faceIndex++;
		}

		for (size_t i = 0; i < 3; i++)
		{
			// Skip the middle one
			if (i == 1)
				continue;

			for (size_t y = 0; y < faceHeight; y++)
			{
				size_t yOffset = y + i * faceHeight;
				for (size_t x = 0; x < faceWidth; x++)
				{
					size_t xOffset = x + faceWidth;
					faces[faceIndex][(x + y * faceWidth) * 3 + 0] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 0];
					faces[faceIndex][(x + y * faceWidth) * 3 + 1] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 1];
					faces[faceIndex][(x + y * faceWidth) * 3 + 2] = m_ImageData[(xOffset + yOffset * m_Width) * 3 + 2];
				}
			}
			faceIndex++;
		}

		EE_RENDER_S3(faces, faceWidth, faceHeight, {
			glGenTextures(1, &self->m_RendererID);
			glBindTexture(GL_TEXTURE_CUBE_MAP, self->m_RendererID);

			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTextureParameterf(self->m_RendererID, GL_TEXTURE_MAX_ANISOTROPY, RendererAPI::GetCapabilities().MaxAnisotropy);

			auto format = ToOpenGLTextureFormat(self->m_Format);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[2]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[0]);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[4]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[5]);

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[1]);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format, faceWidth, faceHeight, 0, format, GL_UNSIGNED_BYTE, faces[3]);

			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

			glBindTexture(GL_TEXTURE_2D, 0);

			for (size_t i = 0; i < faces.size(); i++)
				delete[] faces[i];

			stbi_image_free(self->m_ImageData);
			});
		
	}

	void OpenGLTextureCube::Bind(unsigned slot) const
	{
		EE_RENDER_S1(slot, {
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_CUBE_MAP, self->m_RendererID);
			});
	}

	OpenGLTextureCube::~OpenGLTextureCube()
	{
		EE_RENDER_S({
			glDeleteTextures(1, &self->m_RendererID);
			});
	}
	
}
