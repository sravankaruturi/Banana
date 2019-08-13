#include "eepch.h"
#include "OpenGLIndexBuffer.h"

#include "OpenGL.h"

namespace ee::re
{

	OpenGLIndexBuffer::OpenGLIndexBuffer(euint size)
		: m_RendererID(0), m_Size(size)
	{
		EE_RENDER_S({
			PE_GL(glGenBuffers(1, &self->m_RendererID));
			});
	}

	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		EE_RENDER_S({
			PE_GL(glDeleteBuffers(1, &self->m_RendererID));
			});
	}

	void OpenGLIndexBuffer::SetData(void* buffer, euint size, euint offset)
	{
		EE_RENDER_S3(buffer, size, offset, {
			PE_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_RendererID));
			PE_GL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, buffer, GL_STATIC_DRAW));
			});
	}

	void OpenGLIndexBuffer::Bind() const
	{
		EE_RENDER_S({
			PE_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->m_RendererID));
			});
	}

	void OpenGLIndexBuffer::UnBind() const
	{
		EE_RENDER_S({
			PE_GL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
			});
	}

}
