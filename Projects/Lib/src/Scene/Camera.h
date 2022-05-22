#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Lib
{
	class Camera
	{
	public:
		Camera();
		~Camera();

		void SetPerspective(float verticalFOV, float nearClip, float farClip);

		void SetAspectRatio(float aspectRatio);

		void SetPosition(glm::vec3& pos) { m_CameraPos = pos; CalculateLookAt(); }
		glm::vec3 GetPosition() { return m_CameraPos; }

		void SetRotation(glm::vec3& rotation) { m_CameraRotation = rotation; CalculateLookAt(); }

		const glm::mat4& GetProjection() const { return m_Projection; }
		const glm::mat4& GetView() const { return m_View; }
		const glm::mat4& GetViewProjectiton() const { return m_Projection * m_View; }

	private:
		void CalculatePerspective();
		void CalculateLookAt();
	private:
		glm::mat4 m_Projection = glm::mat4(1.0f);
		glm::mat4 m_View = glm::mat4(1.0f);

		glm::vec3 m_CameraPos = glm::vec3(0.0f);
		glm::vec3 m_CameraRotation = glm::vec3(0.0f);
		glm::vec3 m_CameraZoom = glm::vec3(1.0f);

		float m_VerticalFOV = glm::radians(45.0f);
		float m_NearClip = 0.1f;
		float m_FarClip = 100.0f;

		float m_AspectRatio = (float)800/(float)600;
	};
}