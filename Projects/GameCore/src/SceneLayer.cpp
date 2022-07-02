#include "SceneLayer.h"
#include "Core/Window.h"
#include "Core/Input.h"
#include <iostream>
#include <imgui.h>
#include <filesystem>
#include <string>

SceneLayer::SceneLayer()
{
	m_ActiveScene = std::make_shared<Lib::Scene>();
	m_ActiveScene->AddNewModel("res/models/cube.obj");
}

SceneLayer::~SceneLayer()
{
}

void SceneLayer::AlignCameraWithPlayer(Lib::TransformComponent& cameraTransform, Lib::TransformComponent& playerTransform)
{
	cameraTransform.Translation.z = playerTransform.Translation.z + cameraDistance*
		glm::cos(cameraTransform.Rotation.x)*
		glm::cos(playerTransform.Rotation.y);
	cameraTransform.Translation.x = playerTransform.Translation.x + cameraDistance*
		glm::cos(cameraTransform.Rotation.x)*
		glm::sin(playerTransform.Rotation.y);
	cameraTransform.Translation.y = playerTransform.Translation.y - cameraDistance*
		glm::sin(cameraTransform.Rotation.x);

	cameraTransform.Rotation.y = playerTransform.Rotation.y;

}

void SceneLayer::OnUpdate(float dt, float AspectRatio)
{
	m_ActiveScene->SetAspectRatio(AspectRatio);
	m_ActiveScene->OnUpdate(dt);

	// camera controller

	//TODO : This whole has to be moved to a lua script or a native script component
	auto& editorCamera = m_ActiveScene->GetCurrentCamera();
	auto& currentPlayer = m_ActiveScene->GetCurrentPlayer();
	if (editorCamera)
	{
		float v = 0.001f;
		auto& cameraTransform = editorCamera.GetComponents<Lib::TransformComponent>();
		// controller logic

		if (currentPlayer)
		{
			auto& playerTransform = currentPlayer.GetComponents<Lib::TransformComponent>();
			auto& playerComponent = currentPlayer.GetComponents<Lib::PlayerComponent>();

			if (Lib::Input::IsKeyPressed(Lib::Key::W)) {
				playerTransform.Translation.z -= dt * glm::cos(playerTransform.Rotation.y) * playerComponent.translationalVelocity;
				playerTransform.Translation.x -= dt * glm::sin(playerTransform.Rotation.y) * playerComponent.translationalVelocity;
			}
			else if (Lib::Input::IsKeyPressed(Lib::Key::S)) {
				playerTransform.Translation.z += dt * glm::cos(playerTransform.Rotation.y) * playerComponent.translationalVelocity;
				playerTransform.Translation.x += dt * glm::sin(playerTransform.Rotation.y) * playerComponent.translationalVelocity;
			}

			if (Lib::Input::IsKeyPressed(Lib::Key::A)) {
				playerTransform.Translation.x -= dt * glm::cos(playerTransform.Rotation.y) * playerComponent.translationalVelocity;
				playerTransform.Translation.z += dt * glm::sin(playerTransform.Rotation.y) * playerComponent.translationalVelocity;
				//playerTransform.Rotation.y += dt * playerComponent.rotationalVelocity;
			}
			else if (Lib::Input::IsKeyPressed(Lib::Key::D)) {
				//playerTransform.Rotation.y -= dt * playerComponent.translationalVelocity;
				playerTransform.Translation.x += dt * glm::cos(playerTransform.Rotation.y) * playerComponent.translationalVelocity;
				playerTransform.Translation.z -= dt * glm::sin(playerTransform.Rotation.y) * playerComponent.translationalVelocity;
			}

			if (Lib::Input::IsKeyPressed(Lib::Key::Z))
			{
				if (Lib::Input::IsKeyPressed(Lib::Key::I))
				{
					if (cameraDistance > 0)
						cameraDistance -= 0.02f;
				}
				else if (Lib::Input::IsKeyPressed(Lib::Key::O))
				{
					cameraDistance += 0.02f;
				}
			}

			if (Lib::Input::IsMouseButtonPressed(Lib::Mouse::ButtonLeft) && !MouseLeftButtonPressed)
			{
				MousePosition = Lib::Input::GetMousePosition();
				MouseLeftButtonPressed = true;
				PlayerRotation = { playerTransform.Rotation.x, playerTransform.Rotation.y };
				cameraRotation = { cameraTransform.Rotation.x, cameraTransform.Rotation.y };
			}
			if (!Lib::Input::IsMouseButtonPressed(Lib::Mouse::ButtonLeft) && MouseLeftButtonPressed)
			{
				MousePosition = glm::vec2(0.0f);
				MouseLeftButtonPressed = false;
			}

			if (MouseLeftButtonPressed)
			{
				//cameraTransform.Rotation.y = cameraRotation.y - (MousePosition.x - Lib::Input::GetMousePosition().x) * 0.001;
				cameraTransform.Rotation.x = cameraRotation.x + (MousePosition.y - Lib::Input::GetMousePosition().y) * 0.001;
				playerTransform.Rotation.y = PlayerRotation.y + (MousePosition.x - Lib::Input::GetMousePosition().x) * 0.001;
			}
			AlignCameraWithPlayer(cameraTransform, playerTransform);
		}
		
	}
}

void SceneLayer::OnImGuiRender(float dt)
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
			ImGui::SameLine();
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
			auto modelpathinput = ImGui::InputText("##ModelPath", buf2, IM_ARRAYSIZE(buf2));
			ImGui::SameLine();
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
			ImGui::Text("Light Mode : ");
			ImGui::SameLine();
			if (ImGui::Button("POINT"))
				lightComponent.mode = Lib::LightMode::POINT;
			ImGui::SameLine();
			if (ImGui::Button("DIR"))
			{
				lightComponent.mode = Lib::LightMode::DIR;
				lightComponent.constant = 1.0f;
				lightComponent.linear = 0.0f;
				lightComponent.quadratic = 0.0f;

			}
			ImGui::ColorEdit3("Color", &lightComponent.diffuse.x);
			ImGui::InputFloat("Ambient", &lightComponent.ambient);
			ImGui::InputFloat("Specular", &lightComponent.specular);
			if (lightComponent.mode == Lib::LightMode::POINT)
			{
				ImGui::Text("Attentuation Parameters");
				ImGui::InputFloat("Constant", &lightComponent.constant);
				ImGui::InputFloat("Linear", &lightComponent.linear);
				ImGui::InputFloat("Quadratic", &lightComponent.quadratic);
			}
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

			auto texturepathinput = ImGui::InputText("##TexturePath", texbuf2, IM_ARRAYSIZE(texbuf2));
			ImGui::SameLine();
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

			auto scriptpathinput = ImGui::InputText("##ScriptPath", scriptbuf2, IM_ARRAYSIZE(scriptbuf2));
			ImGui::SameLine();
			if (ImGui::Button("UpdateScript"))
			{
				scriptComponent.m_Path = scriptbuf2;
			}
			ImGui::EndGroup();
		}
		if (m_SelectionContext.HasComponent<Lib::PlayerComponent>())
		{
			auto& currentPlayerPtr = m_SelectionContext.GetComponents<Lib::PlayerComponent>();
			ImGui::Text("Player");
			ImGui::Checkbox("Current Player", &currentPlayerPtr.CurrentPlayer);
			ImGui::SliderFloat("Translation Speed", &currentPlayerPtr.translationalVelocity, 0.0f, 10.0f);
			ImGui::SliderFloat("Rotational Speed", &currentPlayerPtr.rotationalVelocity, 0.0f, 10.0f);
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
			if (ImGui::MenuItem("Player"))
			{
				if (!m_SelectionContext.HasComponent<Lib::PlayerComponent>())
					m_SelectionContext.AddComponent<Lib::PlayerComponent>();
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
	if (ImGui::Button("Add Model"))
	{
		std::string buf3 = Lib::FileDialogs::OpenFile(".obj");
		auto path = std::filesystem::path(buf3);
		std::filesystem::path cwd = std::filesystem::current_path();
		buf3 = std::filesystem::relative(path, cwd).generic_string();

		if (!buf3.empty())
		{
			if (!m_ActiveScene->GetModelFromModelLibrary(buf3))
			{
				m_ActiveScene->AddNewModel(buf3);
			}
		}
	}
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
	{
		std::string filepathbuf = Lib::FileDialogs::SaveFile(".json");
		auto path = std::filesystem::path(filepathbuf);
		std::filesystem::path cwd = std::filesystem::current_path();
		filepathbuf = std::filesystem::relative(path, cwd).generic_string();
		if (!filepathbuf.empty())
		{
			auto& serializer = Lib::SceneSerializer(m_ActiveScene);
			serializer.Serialize(filepathbuf);
			currentscenepath = filepathbuf;
		}
	}
	
	if (ImGui::Button("Open"))
	{
		std::string filepathbuf2 = Lib::FileDialogs::OpenFile(".json");
		auto path = std::filesystem::path(filepathbuf2);
		std::filesystem::path cwd = std::filesystem::current_path();
		filepathbuf2 = std::filesystem::relative(path, cwd).generic_string();
		if (!filepathbuf2.empty())
		{
			auto newScene = std::make_shared<Lib::Scene>();
			auto& serializer = Lib::SceneSerializer(newScene);
			serializer.DeSerialize(filepathbuf2);
			currentscenepath = filepathbuf2;

			m_ActiveScene = newScene;
			m_SelectionContext = {};
		}
	}
	ImGui::End();

	ImGui::Begin("Stats");
	std::string s = "RenderPass : " + std::to_string(m_ActiveScene->GetRenderCount());
	std::string fs = "FrameRate : " + std::to_string((float)1/dt);
	ImGui::Text(s.c_str());
	ImGui::Text(fs.c_str());
	ImGui::End();
}