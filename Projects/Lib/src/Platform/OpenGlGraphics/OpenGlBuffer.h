#pragma once

#include "Renderer/Buffer.h"

namespace Lib
{
	class OpenGlVertexBuffer : public VertexBuffer
	{
	public:
		OpenGlVertexBuffer(const void* vertices, uint32_t size);
		~OpenGlVertexBuffer();

		void Bind() override;
		void UnBind() override;
	private:
		uint32_t m_Id;
	};

	class OpenGlVertexArray : public VertexArray
	{
	public:
		OpenGlVertexArray();
		~OpenGlVertexArray();

		void SetVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer) override;
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer> indexBuffer) override;

		void Bind() override;
		void UnBind() override;
	private:
		uint32_t m_Id;
	};

	class OpenGlIndexBuffer : public IndexBuffer
	{
	public:
		OpenGlIndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGlIndexBuffer();

		void Bind() override;
		void UnBind() override;

		uint32_t GetCount() { return m_Count; }
	private:
		uint32_t m_Id;
		uint32_t m_Count;
	};
}