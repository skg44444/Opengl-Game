#pragma once
#include "Core/ApplicationLayer.h"

#include "Scene/SceneSerializer.h"
#include "Renderer/Shader.h"
#include "Scene/Components.h"

class SceneLayer : public Lib::ApplicationLayer
{
public:
	SceneLayer();
	~SceneLayer();

	void OnUpdate(float dt, float AspectRatio) override;
	void OnImGuiRender(float dt) override;
private:
	void AlignCameraWithPlayer(Lib::TransformComponent& cameraTransform, Lib::TransformComponent& playerTransform);
private:
	std::shared_ptr<Lib::Scene> m_ActiveScene;
	Lib::Entity m_SelectionContext;
	std::string currentscenepath = "";

	bool MouseLeftButtonPressed = false;
		
	glm::vec2 MousePosition = glm::vec2(0.0f);
	glm::vec2 cameraRotation = glm::vec2(0.0f);
	glm::vec2 PlayerRotation = glm::vec2(0.0f);

	float cameraDistance = 1.0f;
};