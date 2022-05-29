#include "SceneLayer.h"
#include <iostream>
#include <imgui.h>

SceneLayer::SceneLayer()
{
	m_ActiveScene = std::make_shared<Lib::Scene>();

	auto& modelentity = m_ActiveScene->CreateEntity("model");
	modelentity.AddComponent<Lib::ModelComponent>(Lib::Model::CreateModel("res/models/twocube.obj"));
	auto& transform = modelentity.GetComponents<Lib::TransformComponent>();
	transform.Translation.z = -3.0f;
	transform.Scale = glm::vec3(0.25f);
	transform.Rotation = glm::vec3(0.0f);

	auto& lightentity = m_ActiveScene->CreateEntity("light");
	lightentity.AddComponent<Lib::ModelComponent>(Lib::Model::CreateModel("res/models/cube.obj"));
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
	for (auto tag : m_ActiveScene->GetEntities())
	{
		auto& entity = m_ActiveScene->GetEntityByTag(tag);
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

	ImGui::Begin("Properties");
	if (m_SelectionContext)
	{
		if (m_SelectionContext.HasComponent<Lib::TagComponent>())
		{
			auto& tag = m_SelectionContext.GetComponents<Lib::TagComponent>();
			ImGui::BeginGroup();
			ImGui::Text("Tag");
			static char buf[256];
			if (ImGui::InputText("Tag", buf, IM_ARRAYSIZE(buf)))
			{
				tag.Tag = buf;
			}
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
	}
	ImGui::End();

	//ImGui::ShowDemoWindow();
	//if (currentEntityTag != "")
	//{
	//	auto& modelentity = m_ActiveScene->GetEntityByTag(currentEntityTag.c_str());
	//	auto& transform = modelentity.GetComponents<Lib::TransformComponent>();
	//
	//	ImGui::Begin("Slider");
	//	ImGui::Text("Transformation");
	//	ImGui::SliderFloat3("Translation", &transform.Translation.x, -10.0f, 10.0f);
	//	ImGui::SliderFloat3("Rotation", &transform.Rotation.x, -10.0f, 10.0f);
	//	ImGui::SliderFloat3("Scale", &transform.Scale.x, 0.0f, 2.0f);
	//	ImGui::End();
	//}
}