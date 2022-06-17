#pragma once

#include "Renderer/Buffer.h"
#include <glm/glm.hpp>

namespace Lib
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normals;
		glm::vec2 TexCoords;

		bool operator==(const Vertex& other) const {
			return Position == other.Position && Normals == other.Normals && TexCoords==other.TexCoords;
		}
	};

	class Model
	{
	public:
		virtual ~Model() = default;

		virtual void Draw() const = 0;

		virtual std::string GetPath() const = 0;

		static std::shared_ptr<Model> CreateModel(const std::string& filepath);
	};
}