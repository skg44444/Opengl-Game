#include "SceneLayer.h"
#include <iostream>
#include <imgui.h>
#include <filesystem>
#include <string>

SceneLayer::SceneLayer()
{
	m_ActiveScene = std::make_shared<Lib::Scene>();
	m_ActiveScene->AddNewModel("res/models/cube.obj");
	//auto& serializer = Lib::SceneSerializer(m_ActiveScene);
	//serializer.DeSerialize("res/scenes/testscene.json");
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
		auto& modelentity = m_ActiveScene->CreateEntity(0, "New Empty Entity");
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
			//strncpy_s(buf, tag.Tag.c_str(), 256);
			auto modified = ImGui::InputText("##Tag", buf, IM_ARRAYSIZE(buf));
			
			if(ImGui::Button("Modify"))
				tag.Tag = buf;
			
			ImGui::EndGroup();
		}
		if (m_SelectionContext.HasComponent<Lib::TransformComponent>())
		{
			auto& transform = m_SelectionContext.GetComponents<Lib::TransformComponent>();

			ImGui::Text("Transformation");
			ImGui::InputFloat3("Translation", &transform.Translation.x);
			ImGui::InputFloat3("Rotation", &transform.Rotation.x);
			ImGui::InputFloat3("Scale", &transform.Scale.x);
		}
		if (m_SelectionContext.HasComponent<Lib::ModelComponent>())
		{
			auto& modelComponent = m_SelectionContext.GetComponents<Lib::ModelComponent>();
			ImGui::BeginGroup();
			std::string mp = "ModelPath : " + modelComponent.m_Path;
			ImGui::Text(mp.c_str());
			static char buf2[256];
			//strncpy_s(buf2, modelComponent.m_Path.c_str(), 256);
			auto modelpathinput = ImGui::InputText("ModelPath", buf2, IM_ARRAYSIZE(buf2));

			if (ImGui::Button("Update"))
			{
				modelComponent.ModelPtr = m_ActiveScene->GetModelFromModelLibrary(buf2);
				modelComponent.m_Path = buf2;
			}
			ImGui::EndGroup();
		}
		if (m_SelectionContext.HasComponent<Lib::LightComponent>())
		{
			auto& lightComponent = m_SelectionContext.GetComponents<Lib::LightComponent>();

			ImGui::Text("Light");
			ImGui::ColorEdit3("Color", &lightComponent.Color.x);
		}
		if (m_SelectionContext.HasComponent<Lib::CameraComponent>())
		{
			auto& currentCameraPtr = m_SelectionContext.GetComponents<Lib::CameraComponent>();
			ImGui::Text("Camera");
			ImGui::Checkbox("Current Camera", &currentCameraPtr.current);
		}
		if (m_SelectionContext.HasComponent<Lib::TextureComponent>())
		{
			auto& textureComponent = m_SelectionContext.GetComponents<Lib::TextureComponent>();
			ImGui::BeginGroup();
			std::string tp = "TexturePath : " + textureComponent.m_Path;
			ImGui::Text(tp.c_str());
			static char texbuf2[256];
			//strncpy_s(texbuf2, textureComponent.m_Path.c_str(), 256);

			auto texturepathinput = ImGui::InputText("TexturePath", texbuf2, IM_ARRAYSIZE(texbuf2));

			if (ImGui::Button("UpdateTexture"))
			{
				textureComponent.m_Path = texbuf2;
				if (!m_ActiveScene->GetTextureFromTextureLibrary(texbuf2))
					m_ActiveScene->AddNewTexture(texbuf2);
			}
			ImGui::EndGroup();
		}
		if (m_SelectionContext.HasComponent<Lib::ScriptComponent>())
		{
			auto& scriptComponent = m_SelectionContext.GetComponents<Lib::ScriptComponent>();
			ImGui::BeginGroup();
			std::string sp = "ScriptPath : " + scriptComponent.m_Path;
			ImGui::Text(sp.c_str());
			static char scriptbuf2[256];

			auto scriptpathinput = ImGui::InputText("ScriptPath", scriptbuf2, IM_ARRAYSIZE(scriptbuf2));

			if (ImGui::Button("UpdateScript"))
			{
				scriptComponent.m_Path = scriptbuf2;
			}
			ImGui::EndGroup();
		}

		if (ImGui::Button("Add Component"))
			ImGui::OpenPopup("AddComponent");
		if (ImGui::BeginPopup("AddComponent"))
		{
			if (ImGui::MenuItem("Camera"))
			{
				if (!m_SelectionContext.HasComponent<Lib::CameraComponent>())
					m_SelectionContext.AddComponent<Lib::CameraComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Light"))
			{
				if (!m_SelectionContext.HasComponent<Lib::LightComponent>())
					m_SelectionContext.AddComponent<Lib::LightComponent>();
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Model"))
			{
				if (!m_SelectionContext.HasComponent<Lib::ModelComponent>())
					m_SelectionContext.AddComponent<Lib::ModelComponent>(m_ActiveScene->GetModelFromModelLibrary("res/models/cube.obj"));
				ImGui::CloseCurrentPopup();
			}
			if (ImGui::MenuItem("Texture"))
			{
				if (!m_SelectionContext.HasComponent<Lib::TextureComponent>())
					m_SelectionContext.AddComponent<Lib::TextureComponent>("");
			}
			if (ImGui::MenuItem("Script"))
			{
				if (!m_SelectionContext.HasComponent<Lib::ScriptComponent>())
					m_SelectionContext.AddComponent<Lib::ScriptComponent>("");
			}
			ImGui::EndPopup();
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
		ImGui::Text("LoadModelPath");
		static char buf3[256];

		auto modelpathinput = ImGui::InputText("LoadModelPath", buf3, IM_ARRAYSIZE(buf3));
		if (ImGui::Button("Add"))
		{
			if (!m_ActiveScene->GetModelFromModelLibrary(buf3))
			{
				m_ActiveScene->AddNewModel(buf3);
				ImGui::CloseCurrentPopup();
			}
		}
		ImGui::EndGroup();
		ImGui::EndPopup();
	}
	ImGui::PopItemWidth();
	for (auto& it = m_ActiveScene->begin(); it != m_ActiveScene->end(); ++it)
	{
		ImGui::Text(it->first.c_str());
	}
	ImGui::End();

	ImGui::Begin("Save");
	if (currentscenepath!= "")
	{
		std::string displaytext = "FileName :" + currentscenepath;
		ImGui::Text(displaytext.c_str());
	}
	if (ImGui::Button("Save"))
	{
		auto& serializer = Lib::SceneSerializer(m_ActiveScene);
		serializer.Serialize(currentscenepath);
	}
	if (ImGui::Button("Save As"))
		ImGui::OpenPopup("SaveAs");
	
	if (ImGui::BeginPopup("SaveAs"))
	{
		ImGui::BeginGroup();
		static char filepathbuf[512];
		auto filepathinput = ImGui::InputText("FilePath", filepathbuf, IM_ARRAYSIZE(filepathbuf));

		if (ImGui::Button("OK"))
		{
			auto& serializer = Lib::SceneSerializer(m_ActiveScene);
			serializer.Serialize(filepathbuf);
			currentscenepath = filepathbuf;
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndGroup();
		ImGui::EndPopup();
	}
	
	if (ImGui::Button("Open"))
		ImGui::OpenPopup("Open");

	if (ImGui::BeginPopup("Open"))
	{
		ImGui::BeginGroup();
		static char filepathbuf2[512];
		auto filepathinput = ImGui::InputText("OpenFilePath", filepathbuf2, IM_ARRAYSIZE(filepathbuf2));

		if (ImGui::Button("Open File"))
		{
			auto newScene = std::make_shared<Lib::Scene>();
			auto& serializer = Lib::SceneSerializer(newScene);
			if (std::filesystem::exists(filepathbuf2))
			{
				serializer.DeSerialize(filepathbuf2);
				currentscenepath = filepathbuf2;

				m_ActiveScene = newScene;
				m_SelectionContext = {};
			}
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndGroup();
		ImGui::EndPopup();
	}
	ImGui::End();

	ImGui::Begin("Stats");
	std::string s = "RenderPass : " + std::to_string(m_ActiveScene->GetRenderCount());
	ImGui::Text(s.c_str());
	ImGui::End();
}