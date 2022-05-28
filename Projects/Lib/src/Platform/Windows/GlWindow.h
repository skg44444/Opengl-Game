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
		float GetAspectRatio() override { return (float)m_Data.width / (float)m_Data.height; }
		void* GetNativeWindow() const { return m_Window; }
	private:
		void OnInit();
		void OnDestroy();
	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			uint32_t width=800, height=600;
			bool IsOpen=true;
		};

		WindowData m_Data;
	};
}