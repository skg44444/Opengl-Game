#pragma once

#include<memory>

namespace Lib
{
	class Window
	{
	public:

		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		virtual bool IsOpen() = 0;

		virtual float GetAspectRatio() = 0;
		virtual bool IsMinimised() = 0;

		virtual void Clear() = 0;

		virtual void* GetNativeWindow() const = 0;

		static std::unique_ptr<Window> CreateWindow();
	};

	class FileDialogs
	{
	public:
		static std::string OpenFile(const char* filter);
		static std::string SaveFile(const char* filter);
	};
}