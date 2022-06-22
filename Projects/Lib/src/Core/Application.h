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
		void* GetNativeWindow() { return m_ApplicationWindow->GetNativeWindow(); }
		static Application* Get() { return s_Instance; }
	private:
		std::unique_ptr<Window> m_ApplicationWindow;

		static Application* s_Instance;

		std::vector<ApplicationLayer*> m_LayerStack;

		float lastframetime = 0.0f;
	};

	Application* CreateApplication();
}

