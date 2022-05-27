#include "SceneLayer.h"

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
	auto& modelentity = m_ActiveScene->GetEntityByTag("model");
	auto& transform = modelentity.GetComponents<Lib::TransformComponent>();
	transform.Rotation.x += 0.005f;
	transform.Rotation.y += 0.005f;
	m_ActiveScene->OnUpdate(dt);
}