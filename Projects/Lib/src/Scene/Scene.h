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
		Entity GetEntityByUUID(const uint64_t uuid);
		void OnUpdate(float dt);
		void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }
		std::vector<std::string> GetEntities();
		std::vector<uint64_t> GetEntityUUIds();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		float m_AspectRatio;
		entt::registry m_Registry;
		friend class Entity;
	};
}