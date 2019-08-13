#include "eepch.h"
#include "OpenGLVertexBuffer.h"

#include "OpenGL.h"

namespace ee::re
{
	OpenGLVertexBuffer::OpenGLVertexBuffer(euint size)
		: m_RendererID(0), m_Size(size)
	{
		EE_RENDER_S({
			PE_GL(glGenBuffers(1, &self->m_RendererID));
			});
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		EE_RENDER_S({
			PE_GL(glDeleteBuffers(1, &self->m_RendererID));
			});
	}

	void OpenGLVertexBuffer::SetData(void* buffer, euint size, euint offset)
	{
		EE_RENDER_S3(
			buffer, size, offset, {
				PE_GL(glBindBuffer(GL_ARRAY_BUFFER, self->m_RendererID));
				PE_GL(glBufferData(GL_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW));

				PE_GL(glEnableVertexAttribArray(0));
				PE_GL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, 0));
			}
		)
	}

	void OpenGLVertexBuffer::Bind() const
	{
		EE_RENDER_S({
			PE_GL(glBindBuffer(GL_ARRAY_BUFFER, self->m_RendererID));
			});
	}

	void OpenGLVertexBuffer::UnBind() const
	{
		EE_RENDER_S({
			PE_GL(glBindBuffer(GL_ARRAY_BUFFER, 0));
			});
	}
}
