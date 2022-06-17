#pragma once

#include "Scene/Components.h"

namespace Lib
{
	enum class RenderMode
	{
		NONE = 0, MODEL, LIGHT, TEXTURE
	};

	class Renderer3D
	{
	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(const Camera& camera, const glm::mat4& cameraView, glm::vec3 lightPos);
		static void EndScene();

		static void Draw(const std::shared_ptr<Model>& model, const TransformComponent& transform, RenderMode mode);
	};
}