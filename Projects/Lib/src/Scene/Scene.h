#pragma once

#include <entt.hpp>

#include "Renderer/Model.h"
#include "Renderer/Texture.h"

namespace Lib
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(uint64_t id = 0, const std::string& name = "Untitled");
		void DestroyEntity(Entity entity);
		Entity GetEntityByTag(const std::string& name);
		Entity GetEntityByUUID(const uint64_t uuid);
		void OnUpdate(float dt);
		void SetAspectRatio(float aspectRatio) { m_AspectRatio = aspectRatio; }
		std::vector<std::string> GetEntities();
		std::vector<uint64_t> GetEntityUUIds();

		void AddNewModel(const std::string& modelpath);
		std::shared_ptr<Lib::Model> GetModelFromModelLibrary(const std::string& modelpath);
		std::unordered_map <std::string, std::shared_ptr<Lib::Model>>::iterator begin() { return ModelLibrary.begin(); }
		std::unordered_map <std::string, std::shared_ptr<Lib::Model>>::iterator end() { return ModelLibrary.end(); }

		void AddNewTexture(const std::string& texturepath);
		std::shared_ptr<Lib::Texture> GetTextureFromTextureLibrary(const std::string& texturepath);

		uint32_t GetRenderCount() { return renderpass; }
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	private:
		float m_AspectRatio;
		entt::registry m_Registry;
		friend class Entity;
		std::unordered_map <std::string, std::shared_ptr<Lib::Model>> ModelLibrary;
		std::unordered_map < std::string, std::shared_ptr<Lib::Texture>> TextureLibrary;
		uint32_t renderpass;
	};
}