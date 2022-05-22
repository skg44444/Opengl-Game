#include "libpch.h"
#include "Scene.h"
#include "Entity.h"
#include "Components.h"

namespace Lib
{
	Scene::Scene()
	{

	}

	Scene::~Scene()
	{

	}

	Entity Scene::CreateEntity(const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(name);
		return entity;
	}
	void Scene::DestroyEntity(Entity entity)
	{
		m_Registry.destroy(entity);
	}

	void Scene::OnUpdate(float dt)
	{
		Camera CurrentCamera;
		glm::mat4 CameraView;
		{
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

				if (camera.current)
				{
					CurrentCamera = camera.SceneCamera;
					CameraView = transform.GetView();
					break;
				}
			}
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<ModelComponent>);
		for (auto entity : group)
		{
			auto [transform, model] = group.get<TransformComponent, ModelComponent>(entity);
			// execute render command
		}
	}
}