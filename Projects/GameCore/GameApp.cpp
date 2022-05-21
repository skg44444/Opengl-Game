#include "Core/Start.h"

class Game : public Lib::Application
{
public:
	Game()
	{

	}

	~Game()
	{

	}
};

Lib::Application* Lib::CreateApplication()
{
	return new Game();
}