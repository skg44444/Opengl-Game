#include "libpch.h"
#include "Window.h"
#include "Platform/Windows/GlWindow.h"

namespace Lib
{
	std::unique_ptr<Window> Window::CreateWindow()
	{
		return std::make_unique<GlWindow>();
	}
}