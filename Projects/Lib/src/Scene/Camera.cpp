#include "libpch.h"
#include "Camera.h"

namespace Lib
{
	Camera::Camera()
	{
		CalculatePerspective();
		CalculateLookAt();
	}

	Camera::~Camera()
	{
	}

	void Camera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		m_VerticalFOV = verticalFOV;
		m_NearClip = nearClip;
		m_FarClip = farClip;
		CalculatePerspective();
	}

	void Camera::SetAspectRatio(float aspectRatio)
	{
		m_AspectRatio = aspectRatio;
		CalculatePerspective();
	}

	void Camera::CalculatePerspective()
	{
		m_Projection = glm::perspective(m_VerticalFOV, m_AspectRatio, m_NearClip, m_FarClip);
	}

	void Camera::CalculateLookAt()
	{
		//glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		//m_View = glm::lookAt(m_CameraPos, m_CameraPos+cameraFront, glm::vec3(0.0f, 1.0f, 0.0f));

		m_View = glm::inverse(glm::translate(glm::mat4(1.0f), m_CameraPos));
	}
}