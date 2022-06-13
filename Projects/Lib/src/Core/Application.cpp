#include "libpch.h"
#include "ImGui/ImGuiLayer.h"
#include "Renderer/Shader.h"
#include "Renderer/Model.h"
#include "Renderer/Renderer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Lib
{
	Application* Application::s_Instance = nullptr;
	Application::Application()
	{
		s_Instance = this;
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
		Renderer3D::Init();

		while (m_ApplicationWindow->IsOpen())
		{
			m_ApplicationWindow->Clear();

			if (!m_ApplicationWindow->IsMinimised())
			{
				for (ApplicationLayer* layer : m_LayerStack)
				{
					layer->OnUpdate(0.0f, m_ApplicationWindow->GetAspectRatio());
				}

				ImGuiLayer::BeginFrame();
				for (ApplicationLayer* layer : m_LayerStack)
				{
					layer->OnImGuiRender();
				}
				ImGuiLayer::EndFrame();
			}

			m_ApplicationWindow->OnUpdate();
		}
	}
}
