#pragma once
#include "Core/Window.h"

#include <GLFW/glfw3.h>

namespace Lib
{
	class GlWindow : public Window
	{
	public:
		GlWindow();
		~GlWindow();

		void Clear() override;
		void OnUpdate() override;
		bool IsOpen() override { return m_Data.IsOpen; }
		bool IsMinimised() override { return m_Data.width == 0 || m_Data.height == 0; }
		float GetAspectRatio() override { return (float)m_Data.width / (float)m_Data.height; }
		void* GetNativeWindow() const { return m_Window; }
	private:
		void OnInit();
		void OnDestroy();
	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			uint32_t width= 1920, height=1080;
			bool IsOpen=true;
		};

		WindowData m_Data;
	};
}