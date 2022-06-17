#pragma once

namespace Lib
{
	class VertexBuffer
	{
	public:
		static std::shared_ptr<VertexBuffer> CreateVertexBuffer(const void* vertices, uint32_t size);
		virtual ~VertexBuffer() = default;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;
	};

	class IndexBuffer
	{
	public:
		static std::shared_ptr<IndexBuffer> CreateIndexBuffer(uint32_t* indices, uint32_t count);
		virtual ~IndexBuffer() = default;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual uint32_t GetCount() = 0;
	};

	class VertexArray
	{
	public:
		static std::shared_ptr<VertexArray> CreateVertexArray();
		virtual ~VertexArray() = default;

		virtual void SetVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer) = 0;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer> indexBuffer) = 0;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;
	};

	class Renderer
	{
	public:
		static void Render(const std::shared_ptr<VertexArray> vao, std::shared_ptr<IndexBuffer>ibo);
	};
}