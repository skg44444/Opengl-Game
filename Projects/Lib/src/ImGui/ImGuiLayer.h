#pragma once

#include "Core/Application.h"

namespace Lib
{
	class ImGuiLayer : public ApplicationLayer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnUpdate(float dt, float AspectRatio);

		static void BeginFrame();
		static void EndFrame();
	};
}