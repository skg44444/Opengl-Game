#pragma once

#include "Renderer/Model.h"
#include "Scene/Camera.h"

namespace Lib
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string & tag)
			: Tag(tag) {}
	};

	struct TransformComponent
	{
		glm::vec3 Translation = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3 & translation)
			: Translation(translation) {}

		glm::mat4 GetTransform()
		{

			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
			
			return glm::translate(glm::mat4(1.0f), Translation) *
				rotation *
				glm::scale(glm::mat4(1.0f), Scale);
		}

		glm::mat4 GetView()
		{
			return glm::inverse(GetTransform());
		}
	};

	struct CameraComponent
	{
		Camera SceneCamera;

		bool current = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct ModelComponent
	{
		std::shared_ptr<Model> ModelPtr = nullptr;

		ModelComponent(const std::shared_ptr<Model> model)
			:ModelPtr(model)
		{

		}
	};
}