#pragma once

#include "Renderer/Model.h"

namespace Lib
{
	class OpenGlMesh
	{
	public:
		OpenGlMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
		~OpenGlMesh();

		void Draw() const;
	private:
		std::shared_ptr<VertexBuffer> m_Vbo;
		std::shared_ptr<IndexBuffer> m_Ibo;
		std::shared_ptr<VertexArray> m_Vao;
	};
	
	class TinyObjModel : public Model
	{
	public:
		TinyObjModel(const std::string& filepath);
		~TinyObjModel();
		void Draw() const override;
		std::string GetPath() const override { return m_Path; }
	private:
		std::vector<std::shared_ptr<OpenGlMesh>> m_Meshes;
		std::string m_Path;
	};
}