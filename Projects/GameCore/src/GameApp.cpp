#include "Core/Start.h"
#include "GameLayer.h"

class Game : public Lib::Application
{
public:
	Game()
	{
		PushLayer(new GameLayer());
	}

	~Game()
	{

	}
};

Lib::Application* Lib::CreateApplication()
{
	return new Game();
}