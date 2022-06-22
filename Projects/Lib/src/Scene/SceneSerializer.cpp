#include "libpch.h"
#include "SceneSerializer.h"

#include <json.hpp>
#include "Components.h"

using json = nlohmann::json;
namespace Lib
{
	static json SerializeEntity(Entity& entity)
	{
		json entitydata;
		entitydata["uuid"] = (uint64_t)entity.GetComponents<TagComponent>().uuid;
		entitydata["name"] = entity.GetComponents<TagComponent>().Tag;

		auto& transformComponent = entity.GetComponents<TransformComponent>();
		entitydata["transform"]["translation"]["x"] = transformComponent.Translation.x;
		entitydata["transform"]["translation"]["y"] = transformComponent.Translation.y;
		entitydata["transform"]["translation"]["z"] = transformComponent.Translation.z;

		entitydata["transform"]["rotation"]["x"] = transformComponent.Rotation.x;
		entitydata["transform"]["rotation"]["y"] = transformComponent.Rotation.y;
		entitydata["transform"]["rotation"]["z"] = transformComponent.Rotation.z;

		entitydata["transform"]["scale"]["x"] = transformComponent.Scale.x;
		entitydata["transform"]["scale"]["y"] = transformComponent.Scale.y;
		entitydata["transform"]["scale"]["z"] = transformComponent.Scale.z;

		if (entity.HasComponent<CameraComponent>())
			entitydata["Camera"]["Current"] = entity.GetComponents<CameraComponent>().current;

		if (entity.HasComponent<LightComponent>())
		{
			auto& lightComponent = entity.GetComponents<LightComponent>();
			entitydata["Light"]["color"]["r"] = lightComponent.diffuse.r;
			entitydata["Light"]["color"]["g"] = lightComponent.diffuse.g;
			entitydata["Light"]["color"]["b"] = lightComponent.diffuse.b;

			entitydata["Light"]["ambient"] = lightComponent.ambient;
			entitydata["Light"]["specular"] = lightComponent.specular;

			entitydata["Light"]["constant"] = lightComponent.constant;
			entitydata["Light"]["linear"] = lightComponent.linear;
			entitydata["Light"]["quadratic"] = lightComponent.quadratic;

			entitydata["Light"]["mode"] = (int)lightComponent.mode;
		}

		if (entity.HasComponent<ModelComponent>())
		{
			entitydata["Model"]["Path"] = entity.GetComponents<ModelComponent>().m_Path;
		}

		if (entity.HasComponent<TextureComponent>())
		{
			entitydata["Texture"]["Path"] = entity.GetComponents<TextureComponent>().m_Path;
		}

		if (entity.HasComponent<ScriptComponent>())
		{
			entitydata["Script"]["Path"] = entity.GetComponents<ScriptComponent>().m_Path;
		}
		return entitydata;
	}

	SceneSerializer::SceneSerializer(std::shared_ptr<Scene> scene)
		:m_Scene(scene)
	{

	}

	SceneSerializer::~SceneSerializer()
	{
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		json j;

		j["Models"] = {};

		for (auto& it = m_Scene->begin(); it != m_Scene->end(); ++it)
		{
			j["Models"].push_back(it->first.c_str());
		}

		j["Entities"] = {};
		
		for (auto uuid : m_Scene->GetEntityUUIds())
		{
			auto& entity = m_Scene->GetEntityByUUID(uuid);


			j["Entities"].push_back(SerializeEntity(entity));
		}

		std::ofstream o(filepath);
		o << std::setw(4) << j << std::endl;
	}

	void SceneSerializer::DeSerialize(const std::string& filepath)
	{
		std::ifstream i(filepath);
		json j;
		i >> j;

		if (j["Models"].is_array())
		{
			for (auto& path : j["Models"])
			{
				m_Scene->AddNewModel(path);
			}
		}
		if (j["Entities"].is_array())
		{
			for (auto& entitydata : j["Entities"])
			{
				uint64_t id = entitydata["uuid"];
				auto name = entitydata["name"];

				auto& entity  = m_Scene->CreateEntity(id, name);
				
				if (entitydata["transform"].is_object())
				{
					auto& transformComponent = entity.GetComponents<TransformComponent>();

					transformComponent.Translation = glm::vec3(entitydata["transform"]["translation"]["x"], 
						entitydata["transform"]["translation"]["y"], entitydata["transform"]["translation"]["z"]);

					transformComponent.Rotation = glm::vec3(entitydata["transform"]["rotation"]["x"],
						entitydata["transform"]["rotation"]["y"], entitydata["transform"]["rotation"]["z"]);

					transformComponent.Scale = glm::vec3(entitydata["transform"]["scale"]["x"],
						entitydata["transform"]["scale"]["y"], entitydata["transform"]["scale"]["z"]);
				}

				if (!entitydata["Camera"].is_null())
				{
					auto& cameraComponent = entity.AddComponent<CameraComponent>();
					if (entitydata["Camera"]["Current"].is_boolean())
						cameraComponent.current = entitydata["Camera"]["Current"];
				}

				if (entitydata["Light"].is_object())
				{
					auto& lightComponent = entity.AddComponent<LightComponent>();
					lightComponent.diffuse = glm::vec3(
						entitydata["Light"]["color"]["r"],
						entitydata["Light"]["color"]["g"],
						entitydata["Light"]["color"]["b"]);

					lightComponent.ambient = entitydata["Light"]["ambient"];
					lightComponent.specular = entitydata["Light"]["specular"];
					
					lightComponent.constant = entitydata["Light"]["constant"];
					lightComponent.linear = entitydata["Light"]["linear"];
					lightComponent.quadratic = entitydata["Light"]["quadratic"];
					
					lightComponent.mode = (LightMode)entitydata["Light"]["mode"];
				}

				if (entitydata["Model"].is_object())
				{
					auto& modelComponent = entity.AddComponent<ModelComponent>(m_Scene->GetModelFromModelLibrary(entitydata["Model"]["Path"]));
				}

				if (entitydata["Texture"].is_object())
				{
					auto& textureComponent = entity.AddComponent<TextureComponent>(entitydata["Texture"]["Path"]);
					if (!m_Scene->GetTextureFromTextureLibrary(entitydata["Texture"]["Path"]))
						m_Scene->AddNewTexture(entitydata["Texture"]["Path"]);
				}

				if (entitydata["Script"].is_object())
				{
					auto& scriptComponent = entity.AddComponent<ScriptComponent>(entitydata["Script"]["Path"]);
				}
			}
		}
	}
}