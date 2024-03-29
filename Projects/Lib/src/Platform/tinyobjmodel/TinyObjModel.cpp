#include "libpch.h"
#include "TinyObjModel.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>


namespace std {
	template<> struct hash<Lib::Vertex> {
		size_t operator()(Lib::Vertex const& vertex) const {
			return ((hash<glm::vec3>()(vertex.Position) ^
				(hash<glm::vec3>()(vertex.Normals) << 1)) >> 1) ^
				(hash<glm::vec2>()(vertex.TexCoords) << 1);
		}
	};
}

namespace Lib
{
	TinyObjModel::TinyObjModel(const std::string& filepath)
		:m_Path(filepath)
	{
		tinyobj::ObjReaderConfig reader_config;
		reader_config.mtl_search_path = "res/models"; // Path to material files

		tinyobj::ObjReader reader;

		if (!reader.ParseFromFile(filepath, reader_config)) {
			if (!reader.Error().empty()) {
				std::cerr << "TinyObjReader: " << reader.Error();
			}
		}

		if (!reader.Warning().empty()) {
			std::cout << "TinyObjReader: " << reader.Warning();
		}

		auto& attrib = reader.GetAttrib();
		auto& shapes = reader.GetShapes();
		auto& materials = reader.GetMaterials();


		for (const auto& shape : shapes)
		{
			std::vector<Vertex> vertices;
			std::vector<uint32_t> indices;
			std::unordered_map<Vertex, uint32_t> uniqueVertices{};

			for (const auto& index : shape.mesh.indices)
			{
				Vertex vertex{};

				vertex.Position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

				vertex.Normals = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2]
				};

				vertex.TexCoords = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1]
				};

				if (uniqueVertices.count(vertex) == 0) {
					uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
					vertices.push_back(vertex);
				}

				indices.push_back(uniqueVertices[vertex]);
			}

			m_Meshes.push_back(std::make_shared<OpenGlMesh>(vertices, indices));
		}
	}

	TinyObjModel::~TinyObjModel()
	{
		std::string base_filename = m_Path.substr(m_Path.find_last_of("/") + 1);
		std::cout << "Deleting " << base_filename << std::endl;
	}

	void TinyObjModel::Draw() const
	{
		for (auto& mesh : m_Meshes)
		{
			mesh->Draw();
		}
	}
	
	OpenGlMesh::OpenGlMesh(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
	{
		m_Vao = VertexArray::CreateVertexArray();
		m_Vbo = VertexBuffer::CreateVertexBuffer(&vertices[0], sizeof(Vertex) * vertices.size());
		m_Vao->SetVertexBuffer(m_Vbo);
		m_Ibo = IndexBuffer::CreateIndexBuffer(&indices[0], indices.size());
		m_Vao->SetIndexBuffer(m_Ibo);
	}

	OpenGlMesh::~OpenGlMesh()
	{
		//std::cout << "Deleting Mesh" << std::endl;
	}

	void OpenGlMesh::Draw() const
	{
		Renderer::Render(m_Vao, m_Ibo);
	}

	/*void OpenGlMesh::SetupMesh()
	{
	}*/

	TerrainModel::TerrainModel()
	{
	}

	TerrainModel::~TerrainModel()
	{
	}

	void TerrainModel::Draw() const
	{
	}

}