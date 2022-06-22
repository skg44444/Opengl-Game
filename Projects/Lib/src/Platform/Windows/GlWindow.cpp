#include "libpch.h"
#include "GlWindow.h"

#include <glad/glad.h>

namespace Lib
{
	GlWindow::GlWindow()
	{
		OnInit();
	}

	GlWindow::~GlWindow()
	{
		OnDestroy();
	}

	void GlWindow::OnInit()
	{
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		m_Window = glfwCreateWindow(m_Data.width, m_Data.height, "OpenGL Window", nullptr, nullptr);

		if (m_Window == NULL)
		{
			std::cout << "Failed to create Window" << std::endl;
			glfwTerminate();
		}

		glfwMakeContextCurrent(m_Window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
		}

		glViewport(0, 0, m_Data.width, m_Data.height);
		glEnable(GL_DEPTH_TEST);
		glfwSwapInterval(0);
		// Print GPU Details
		std::cout << "GPU VENDOR : [" << glGetString(GL_VENDOR) << "]" << std::endl;

		// set callbacks

		glfwSetWindowUserPointer(m_Window, &m_Data);


		glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.width = width;
				data.height = height;

				glViewport(0, 0, width, height);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.IsOpen = false;
			});

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.width = width;
				data.height = height;
			});
	}

	void GlWindow::Clear()
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void GlWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void GlWindow::OnDestroy()
	{
		glfwTerminate();
	}
}