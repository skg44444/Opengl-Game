#pragma once

#include "Entity.h"

namespace Lib
{
	class SceneSerializer
	{
	public:
		SceneSerializer(std::shared_ptr<Scene> scene);
		~SceneSerializer();

		void Serialize(const std::string& filepath);
		void DeSerialize(const std::string& filepath);
	private:
		std::shared_ptr<Scene> m_Scene;
	};
}