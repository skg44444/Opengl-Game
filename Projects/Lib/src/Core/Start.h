#include "libpch.h"

#include "Application.h"

extern Lib::Application* Lib::CreateApplication();

int main()
{
	auto app = Lib::CreateApplication();

	app->Run();

	delete app;

	std::cin.get();
}