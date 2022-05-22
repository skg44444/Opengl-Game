#pragma once

#include <entt.hpp>

namespace Lib
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = "Untitled");
		void DestroyEntity(Entity entity);

		void OnUpdate(float dt);
	private:
		entt::registry m_Registry;
		friend class Entity;
	};
}