#pragma once
#include "Core/ApplicationLayer.h"

class GameLayer : public Lib::ApplicationLayer
{
public:
	GameLayer();
	~GameLayer();

	void OnUpdate(float dt) override;
};