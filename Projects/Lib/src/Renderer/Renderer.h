#pragma once

#include "Scene/Components.h"

namespace Lib
{
	enum class RenderMode
	{
		NONE = 0, MODEL, LIGHT, TEXTURE
	};

	struct LightData
	{
		glm::vec3 lightPos = glm::vec3(0.0f);
		glm::vec3 lightColor = glm::vec3(0.0f);
		
		float ambient = 0.2f;
		float specular = 10;
		
		float constant = 1.0f;
		float linear = 0.0f;
		float quadratic = 0.0f;
	};

	class Renderer3D
	{
	public:
		static void Init();
		static void ShutDown();

		static void BeginScene(const Camera& camera, const glm::mat4& cameraView, const std::vector<LightData>& lightDataArray);
		static void EndScene();

		static void Draw(const std::shared_ptr<Model>& model, const TransformComponent& transform, RenderMode mode);
	};
}