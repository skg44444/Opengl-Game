#include "libpch.h"
#include "Application.h"
#include "Renderer/Shader.h"
#include "Renderer/Model.h"
#include "Scene/Components.h"

namespace Lib
{
	Application::Application()
	{
		m_ApplicationWindow = Window::CreateWindow();
	}

	Application::~Application()
	{
		for (ApplicationLayer* layer : m_LayerStack)
		{
			delete layer;
		}
	}

	void Application::Run()
	{		
		while (m_ApplicationWindow->IsOpen())
		{
			m_ApplicationWindow->Clear();

			for (ApplicationLayer* layer : m_LayerStack)
			{
				layer->OnUpdate(0.0f, m_ApplicationWindow->GetAspectRatio());
			}

			m_ApplicationWindow->OnUpdate();
		}
	}
}
