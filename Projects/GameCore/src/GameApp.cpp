#include "Core/Start.h"
#include "SceneLayer.h"

class Game : public Lib::Application
{
public:
	Game()
	{
		PushLayer(new SceneLayer());
	}

	~Game()
	{

	}
};

Lib::Application* Lib::CreateApplication()
{
	return new Game();
}