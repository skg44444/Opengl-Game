#pragma once

namespace Lib
{
	class ApplicationLayer
	{
	public:
		ApplicationLayer() = default;
		virtual ~ApplicationLayer() = default;

		virtual void OnUpdate(float dt, float AspectRatio) = 0;
	};
}