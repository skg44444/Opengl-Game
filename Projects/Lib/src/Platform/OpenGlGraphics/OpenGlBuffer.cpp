#include "libpch.h"

#include "OpenGlBuffer.h"
#include<glad/glad.h>

namespace Lib
{
	OpenGlVertexBuffer::OpenGlVertexBuffer(const void* vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_Id);
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	OpenGlVertexBuffer::~OpenGlVertexBuffer()
	{
		glDeleteBuffers(1, &m_Id);
	}

	void OpenGlVertexBuffer::Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
	}

	void OpenGlVertexBuffer::UnBind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	OpenGlVertexArray::OpenGlVertexArray()
	{
		glCreateVertexArrays(1, &m_Id);
	}

	OpenGlVertexArray::~OpenGlVertexArray()
	{
		glDeleteVertexArrays(1, &m_Id);
	}

	void OpenGlVertexArray::SetVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer)
	{
		glBindVertexArray(m_Id);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	}

	void OpenGlVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> indexBuffer)
	{
	}

	void OpenGlVertexArray::Bind()
	{
		glBindVertexArray(m_Id);
	}

	void OpenGlVertexArray::UnBind()
	{
		glBindVertexArray(0);
	}


	OpenGlIndexBuffer::OpenGlIndexBuffer(uint32_t* indices, uint32_t count)
		:m_Count(count)
	{
		glGenBuffers(1, &m_Id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * m_Count, indices, GL_STATIC_DRAW);
	}

	OpenGlIndexBuffer::~OpenGlIndexBuffer()
	{
		glDeleteBuffers(1, &m_Id);
	}

	void OpenGlIndexBuffer::Bind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
	}

	void OpenGlIndexBuffer::UnBind()
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}


}