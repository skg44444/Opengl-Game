#include "SceneLayer.h"
#include <iostream>
#include <imgui.h>

SceneLayer::SceneLayer()
{
	m_ActiveScene = std::make_shared<Lib::Scene>();

	ModelLibrary["res/models/twocube.obj"] = Lib::Model::CreateModel("res/models/twocube.obj");
	ModelLibrary["res/models/cube.obj"] = Lib::Model::CreateModel("res/models/cube.obj");

	auto& modelentity = m_ActiveScene->CreateEntity("model");
	modelentity.AddComponent<Lib::ModelComponent>(ModelLibrary["res/models/twocube.obj"]);
	auto& transform = modelentity.GetComponents<Lib::TransformComponent>();
	transform.Translation.z = -3.0f;
	transform.Scale = glm::vec3(0.25f);
	transform.Rotation = glm::vec3(0.0f);

	auto& lightentity = m_ActiveScene->CreateEntity("light");
	lightentity.AddComponent<Lib::ModelComponent>(ModelLibrary["res/models/cube.obj"]);
	lightentity.AddComponent<Lib::LightComponent>();
	auto& lightTransform = lightentity.GetComponents<Lib::TransformComponent>();
	lightTransform.Translation = glm::vec3(-1.0f, 1.0f, -3.0f);
	lightTransform.Scale = glm::vec3(0.1f);

	auto& cameraentity = m_ActiveScene->CreateEntity("camera");
	cameraentity.AddComponent<Lib::CameraComponent>();
	auto& cameratransform = cameraentity.GetComponents<Lib::TransformComponent>();
}

SceneLayer::~SceneLayer()
{
}

void SceneLayer::OnUpdate(float dt, float AspectRatio)
{
	m_ActiveScene->SetAspectRatio(AspectRatio);
	m_ActiveScene->OnUpdate(dt);
}

void SceneLayer::OnImGuiRender()
{
	ImGui::Begin("Entities");
	if (ImGui::Button("New Entity"))
		auto& modelentity = m_ActiveScene->CreateEntity("New Empty Entity");
	for (auto uuid : m_ActiveScene->GetEntityUUIds())
	{
		auto& entity = m_ActiveScene->GetEntityByUUID(uuid);
		auto& tag = entity.GetComponents<Lib::TagComponent>().Tag;
		
		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

		flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}
	ImGui::End();

	if (m_SelectionContext)
	{
		ImGui::Begin("Properties");
		if (m_SelectionContext.HasComponent<Lib::TagComponent>())
		{
			auto& tag = m_SelectionContext.GetComponents<Lib::TagComponent>();
			ImGui::BeginGroup();
			ImGui::Text("Tag");
			static char buf[256];
			auto modified = ImGui::InputText("##Tag", buf, IM_ARRAYSIZE(buf));
			
			if(ImGui::Button("Modify"))
				tag.Tag = buf;
			
			ImGui::EndGroup();
		}
		if (m_SelectionContext.HasComponent<Lib::TransformComponent>())
		{
			auto& transform = m_SelectionContext.GetComponents<Lib::TransformComponent>();

			ImGui::Text("Transformation");
			ImGui::SliderFloat3("Translation", &transform.Translation.x, -10.0f, 10.0f);
			ImGui::SliderFloat3("Rotation", &transform.Rotation.x, -10.0f, 10.0f);
			ImGui::SliderFloat3("Scale", &transform.Scale.x, 0.0f, 2.0f);
		}
		if (!m_SelectionContext.HasComponent<Lib::ModelComponent>())
		{
			ImGui::BeginGroup();
			ImGui::Text("ModelPath");
			static char buf2[256];

			auto modelpathinput = ImGui::InputText("ModelPath", buf2, IM_ARRAYSIZE(buf2));
			if (ImGui::Button("Set"))
			{
				if (!ModelLibrary[buf2])
				{
					ModelLibrary[buf2] = Lib::Model::CreateModel(buf2);
				}

				m_SelectionContext.AddComponent<Lib::ModelComponent>(ModelLibrary[buf2]);
			}
			ImGui::EndGroup();
		}
		else
		{
			auto& modelPath = m_SelectionContext.GetComponents<Lib::ModelComponent>().m_Path;
			ImGui::BeginGroup();
			ImGui::Text("ModelPath");
			ImGui::Text(modelPath.c_str());
			ImGui::EndGroup();
		}
		if (ImGui::Button("Destroy"))
		{
			m_ActiveScene->DestroyEntity(m_SelectionContext);
			m_SelectionContext = {};
		}
		ImGui::End();
	}

	ImGui::Begin("Models");
	ImGui::SameLine();
	ImGui::PushItemWidth(-1);
	if (ImGui::Button("Add Model"))
		ImGui::OpenPopup("AddModel");
	if (ImGui::BeginPopup("AddModel"))
	{
		ImGui::BeginGroup();
		ImGui::Text("ModelPath");
		static char buf3[256];

		auto modelpathinput = ImGui::InputText("ModelPath", buf3, IM_ARRAYSIZE(buf3));
		if (ImGui::Button("Add"))
		{
			//std::string modelpath(buf3);
			if (!ModelLibrary[buf3])
			{
				ModelLibrary[buf3] = Lib::Model::CreateModel(buf3);
				ImGui::CloseCurrentPopup();
			}
			//tag.Tag = buf;
		}
		ImGui::EndGroup();
		ImGui::EndPopup();
	}
	ImGui::PopItemWidth();
	for (auto& [key, value] : ModelLibrary)
	{
		ImGui::Text(key.c_str());
	}
	ImGui::End();
}