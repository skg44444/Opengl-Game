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
	std::shared_ptr<Lib::Scene> m_ActiveScene;
	Lib::Entity m_SelectionContext;
	std::string currentscenepath = "";
};