#include "libpch.h"
#include "Scene.h"
#include "Entity.h"
#include "Renderer/Renderer.h"
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

	Entity Scene::GetEntityByTag(const std::string& name)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const auto& entityname = view.get<TagComponent>(entity).Tag;
			if (entityname == name)
			{
				return Entity{ entity, this };
			}
		}
	}

	Entity Scene::GetEntityByUUID(const uint64_t uuid)
	{
		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
		{
			const auto& entityid = (uint64_t)view.get<TagComponent>(entity).uuid;
			if (entityid == uuid)
			{
				return Entity{ entity, this };
			}
		}
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
		glm::vec3 LightPos;
		{
			auto view = m_Registry.view<TransformComponent, LightComponent>();

			for (auto entity : view)
			{
				auto transform = view.get<TransformComponent>(entity);
				LightPos = transform.Translation;
				break;
			}
		}
		CurrentCamera.SetAspectRatio(m_AspectRatio);
		Renderer3D::BeginScene(CurrentCamera, CameraView, LightPos);
		auto lightgroup = m_Registry.group<TransformComponent>(entt::get<ModelComponent, LightComponent>);
		for (auto entity : lightgroup)
		{
			auto [transform, model, light] = lightgroup.get<TransformComponent, ModelComponent, LightComponent>(entity);
			Renderer3D::Draw(model.ModelPtr, transform, RenderMode::LIGHT);
		}
		auto group = m_Registry.group<TransformComponent>(entt::get<ModelComponent>);
		for (auto entity : group)
		{
			if (!m_Registry.any_of<LightComponent>(entity))
			{
				auto [transform, model] = group.get<TransformComponent, ModelComponent>(entity);
				// execute render command
				Renderer3D::Draw(model.ModelPtr, transform, RenderMode::MODEL);
			}
		}
		Renderer3D::EndScene();
	}

	std::vector<std::string> Scene::GetEntities()
	{
		std::vector<std::string> listOfEntities;

		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
			listOfEntities.push_back(view.get<TagComponent>(entity).Tag);

		return listOfEntities;
	}

	std::vector<uint64_t> Scene::GetEntityUUIds()
	{
		std::vector<uint64_t> listOfEntities;

		auto view = m_Registry.view<TagComponent>();
		for (auto entity : view)
			listOfEntities.push_back(view.get<TagComponent>(entity).uuid);

		return listOfEntities;
	}

	template<typename T>
	void Scene::OnComponentAdded(Entity entity, T& component)
	{

	}

	template<>
	void Scene::OnComponentAdded(Entity entity, TransformComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded(Entity entity, ModelComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded(Entity entity, TagComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded(Entity entity, LightComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded(Entity entity, CameraComponent& component)
	{

	}
}