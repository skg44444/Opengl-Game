#include "libpch.h"

#include "Platform/OpenGlGraphics/OpenGlBuffer.h"

#include "glad/glad.h"

namespace Lib
{
	std::shared_ptr<VertexBuffer> VertexBuffer::CreateVertexBuffer(const void* vertices, uint32_t size)
	{
		return std::make_shared<OpenGlVertexBuffer>(vertices, size);
	}

	std::shared_ptr<VertexArray> VertexArray::CreateVertexArray()
	{
		return std::make_shared<OpenGlVertexArray>();
	}

	std::shared_ptr<IndexBuffer> IndexBuffer::CreateIndexBuffer(uint32_t* indices, uint32_t count)
	{
		return std::make_shared<OpenGlIndexBuffer>(indices, count);
	}
	void Renderer::Render(const std::shared_ptr<VertexArray> vao, std::shared_ptr<IndexBuffer>ibo)
	{
		vao->Bind();
		ibo->Bind();
		glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, nullptr);
	}
}