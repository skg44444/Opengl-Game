#pragma once

#include "Renderer/Buffer.h"
#include <glm/glm.hpp>

namespace Lib
{
	struct Vertex
	{
		glm::vec3 Position;

		bool operator==(const Vertex& other) const {
			return Position == other.Position;
		}
	};

	class Model
	{
	public:
		virtual ~Model() = default;

		virtual void Draw() const = 0;

		static std::shared_ptr<Model> CreateModel(const std::string& filepath);
	};
}