#pragma once
#include "Window.h"
#include "ApplicationLayer.h"

namespace Lib
{
	class Application
	{
	public:
		Application();
		~Application();

		void PushLayer(ApplicationLayer* layer) { m_LayerStack.push_back(layer); }

		void Run();
	private:
		std::unique_ptr<Window> m_ApplicationWindow;

		std::vector<ApplicationLayer*> m_LayerStack;
	};

	Application* CreateApplication();
}

