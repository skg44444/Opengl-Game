#pragma once
#include "Core/ApplicationLayer.h"

#include <string>
#include <memory>

#include "Renderer/Shader.h"
#include "Scene/Components.h"

class GameLayer : public Lib::ApplicationLayer
{
public:
	GameLayer();
	~GameLayer();

	void OnUpdate(float dt, float AspectRatio) override;
private:
	std::shared_ptr<Lib::Shader> shader;
	std::shared_ptr<Lib::Shader> lightShader;
	std::shared_ptr<Lib::Model> model;
	std::shared_ptr<Lib::Model> cubemodel;
	
	Lib::TransformComponent transform;
	Lib::TransformComponent cubeTransform;
	Lib::CameraComponent camera;
};