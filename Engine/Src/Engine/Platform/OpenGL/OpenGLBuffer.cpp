#include "eepch.h"
#include "OpenGLBuffer.h"

#include <glad/glad.h>

namespace ee::re
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(euint size)
		: m_RendererID(0), m_Size(size)
	{
		EE_RENDER_S({
			glGenBuffers(1, &self->m_RendererID);
			});
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		EE_RENDER_S({
			glDeleteBuffers(1, &self->m_RendererID);
			});
	}

	void OpenGLVertexBuffer::SetData(void* buffer, euint size, euint offset)
	{
		m_Size = size;
		EE_RENDER_S3(
			buffer, size, offset, {
				glBindBuffer(GL_ARRAY_BUFFER, self->m_RendererID);
				glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);

				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0);
			}
		)
	}

	void OpenGLVertexBuffer::Bind() const
	{
		EE_RENDER_S({
			glBindBuffer(GL_ARRAY_BUFFER, self->m_RendererID);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (const void*)(3 * sizeof(float)));
			
			});
	}

	void OpenGLVertexBuffer::UnBind() const
	{
		EE_RENDER_S({
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			});
	}

	OpenGLIndexBuffer::OpenGLIndexBuffer(euint size)
		: m_RendererID(0), m_Size(size)
	{
		EE_RENDER_S({
			glGenBuffers(1, &self->m_RendererID);
			});
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		EE_RENDER_S({
			glDeleteBuffers(1, &self->m_RendererID);
			});
	}

	void OpenGLIndexBuffer::SetData(void* buffer, euint size, euint offset)
	{

		m_Size = size;
		
		EE_RENDER_S3(buffer, size, offset, {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_RendererID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW);
			});
	}

	void OpenGLIndexBuffer::Bind() const
	{
		EE_RENDER_S({
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_RendererID);
			});
	}

	void OpenGLIndexBuffer::UnBind() const
	{
		EE_RENDER_S({
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			});
	}
	
}
