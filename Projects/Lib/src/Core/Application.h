#pragma once
#include "Window.h"

namespace Lib
{
	class Application
	{
	public:
		Application();
		~Application();

		void Run();
	private:
		std::unique_ptr<Window> m_ApplicationWindow;
	};

	Application* CreateApplication();
}

