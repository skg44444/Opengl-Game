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
		Entity GetEntityByTag(const std::string& name);
		void OnUpdate(float dt);
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		entt::registry m_Registry;
		friend class Entity;
	};
}