#include "Core/Start.h"
#include "SceneLayer.h"
#include "ImGui/ImGuiLayer.h"
class Game : public Lib::Application
{
public:
	Game()
	{
		PushLayer(new SceneLayer());
		PushLayer(new Lib::ImGuiLayer());
	}

	~Game()
	{

	}
};

Lib::Application* Lib::CreateApplication()
{
	return new Game();
}