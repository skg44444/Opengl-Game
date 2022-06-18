#include "libpch.h"
#include "Renderer/Renderer.h"
#include "Utils/LuaWrappers.h"

namespace Lib
{

	static bool checkLua(lua_State* L, int r)
	{
		if (r != LUA_OK)
		{
			std::string errormsg = lua_tostring(L, -1);
			std::cout << errormsg << std::endl;
			return false;
		}
		return true;
	}

    bool Scene::executeLuaFile(const std::string& filepath, LuaWrappers* wrapperObject)
	{
		if (checkLua(L, luaL_dofile(L, filepath.c_str())))
		{
			lua_getglobal(L, "Execute");
			if (lua_isfunction(L, -1))
			{
				lua_pushlightuserdata(L, wrapperObject);
				if (checkLua(L, lua_pcall(L, 1, 0, 0)))
				{
					return true;
				}
			}
		}
		return false;
	}

	Scene::Scene()
		:renderpass(0), m_AspectRatio(1)
	{
		m_ScriptWrapper = new LuaWrappers();
		L = luaL_newstate();
		luaL_openlibs(L);

		lua_register(L, "_Sum", m_ScriptWrapper->wrap_Sum);
		lua_register(L, "_Move", m_ScriptWrapper->wrap_Move);

		//executeLuaFile("filename.lua");
	}

	Scene::~Scene()
	{
		lua_close(L);
		delete m_ScriptWrapper;
	}

	Entity Scene::CreateEntity(uint64_t id, const std::string& name)
	{
		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		if (id == 0)
			entity.AddComponent<TagComponent>(name);
		else
			entity.AddComponent<TagComponent>(name, id);
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

		return {};
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

		return {};
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
		glm::vec3 LightPos = glm::vec3(NULL);
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
		//auto lightgroup = m_Registry.group<TransformComponent>(entt::get<ModelComponent, LightComponent>);
		//for (auto entity : lightgroup)
		//{
		//	auto [transform, model, light] = lightgroup.get<TransformComponent, ModelComponent, LightComponent>(entity);
		//	Renderer3D::Draw(model.ModelPtr, transform, RenderMode::LIGHT);
		//}
		//auto group = m_Registry.group<TransformComponent>(entt::get<ModelComponent>);
		//for (auto entity : group)
		//{
		//	if (!m_Registry.any_of<LightComponent>(entity))
		//	{
		//		auto [transform, model] = group.get<TransformComponent, ModelComponent>(entity);
		//		// execute render command
		//		Renderer3D::Draw(model.ModelPtr, transform, RenderMode::MODEL);
		//	}
		//}
		renderpass = 0;
		auto scriptgroup = m_Registry.group<ScriptComponent>(entt::get<TagComponent>);
		for (auto entity:scriptgroup) 
		{
			auto [script, tag] = scriptgroup.get<ScriptComponent, TagComponent>(entity);

			m_ScriptWrapper->SetEntity(GetEntityByUUID(tag.uuid));

			if (script.m_Path != "")
				if (!executeLuaFile(script.m_Path, m_ScriptWrapper))
					std::cout << "ERROR" << std::endl;
		}

		auto lightgroup = m_Registry.group<LightComponent>(entt::get<ModelComponent, TransformComponent>);
		for (auto entity : lightgroup)
		{
			auto [transform, model, light] = lightgroup.get<TransformComponent, ModelComponent, LightComponent>(entity);
			Renderer3D::Draw(model.ModelPtr, transform, RenderMode::LIGHT);
			renderpass++;
		}

		auto texgroup = m_Registry.group<TextureComponent>(entt::get<ModelComponent, TransformComponent>);
		for (auto entity : texgroup)
		{
			auto [transform, model, texture] = texgroup.get<TransformComponent, ModelComponent, TextureComponent>(entity);
			// execute render command
			if (texture.m_Path != "")
			{
				TextureLibrary[texture.m_Path]->Bind();
				Renderer3D::Draw(model.ModelPtr, transform, RenderMode::TEXTURE);
			}
			else
			{
				Renderer3D::Draw(model.ModelPtr, transform, RenderMode::MODEL);
			}
			renderpass++;
		}

		auto modelgroup = m_Registry.group<ModelComponent>(entt::get<TransformComponent>, entt::exclude<TextureComponent>);
		for (auto entity : modelgroup)
		{
			if (!m_Registry.any_of<LightComponent>(entity))
			{
				auto [transform, model] = modelgroup.get<TransformComponent, ModelComponent>(entity);
				// execute render command
				Renderer3D::Draw(model.ModelPtr, transform, RenderMode::MODEL);
				renderpass++;
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

	void Scene::AddNewModel(const std::string& modelpath)
	{
		if(!ModelLibrary[modelpath])
			ModelLibrary[modelpath] = Model::CreateModel(modelpath);
	}

	std::shared_ptr<Lib::Model> Scene::GetModelFromModelLibrary(const std::string& modelpath)
	{
		if (ModelLibrary[modelpath])
			return ModelLibrary[modelpath];
		else
			return false;
	}

	void Scene::AddNewTexture(const std::string& texturepath)
	{
		if (!TextureLibrary[texturepath])
			TextureLibrary[texturepath] = Texture::CreateTexture(texturepath);
	}

	std::shared_ptr<Lib::Texture> Scene::GetTextureFromTextureLibrary(const std::string& texturepath)
	{
		if (TextureLibrary[texturepath])
			return TextureLibrary[texturepath];
		else
			return false;
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

	template<>
	void Scene::OnComponentAdded(Entity entity, TextureComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded(Entity entity, ScriptComponent& component)
	{
	}
}