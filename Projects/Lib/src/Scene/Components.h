#pragma once

#include "Renderer/Model.h"
#include "Scene/Camera.h"
#include "Core/UUID.h"

namespace Lib
{
	struct TagComponent
	{
		std::string Tag;
		UUID uuid;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string & tag)
			: Tag(tag), uuid(UUID()) {}

		TagComponent(const std::string& tag, uint64_t id)
			: Tag(tag), uuid(id)
		{}
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

		glm::mat4 GetTransform() const
		{

			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
			
			return glm::translate(glm::mat4(1.0f), Translation) *
				rotation *
				glm::scale(glm::mat4(1.0f), Scale);
		}

		glm::mat4 GetView() const
		{
			return glm::inverse(GetTransform());
		}
	};

	struct CameraComponent
	{
		Camera SceneCamera;

		bool current = true;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	struct ModelComponent
	{
		std::shared_ptr<Model> ModelPtr = nullptr;
		std::string m_Path;

		ModelComponent() = default;
		ModelComponent(const ModelComponent&) = default;
		ModelComponent(const std::shared_ptr<Model>& model)
			:ModelPtr(model), m_Path(model->GetPath())
		{

		}
	};

	struct LightComponent
	{
		glm::vec3 Color = glm::vec3(1.0f);

		LightComponent() = default;
		LightComponent(const LightComponent&) = default;
		LightComponent(const glm::vec3 & color)
			: Color(color) {}
	};
}