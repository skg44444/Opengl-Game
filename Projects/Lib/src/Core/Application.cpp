#include "libpch.h"
#include "Application.h"
#include "Renderer/Shader.h"
#include "Renderer/Model.h"
#include "Renderer/Renderer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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
		Renderer3D::Init();

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		// Setup Platform/Renderer backends
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)m_ApplicationWindow->GetNativeWindow(), true);
		ImGui_ImplOpenGL3_Init("#version 450");


		while (m_ApplicationWindow->IsOpen())
		{
			m_ApplicationWindow->Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			bool show = false;
			ImGui::ShowDemoWindow(&show);

			for (ApplicationLayer* layer : m_LayerStack)
			{
				layer->OnUpdate(0.0f, m_ApplicationWindow->GetAspectRatio());
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			m_ApplicationWindow->OnUpdate();
		}

		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
}
