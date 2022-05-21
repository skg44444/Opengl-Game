#include "libpch.h"
#include "Application.h"
#include "Renderer/Shader.h"
#include "Renderer/Model.h"
#include "Scene/Components.h"

#include <glad/glad.h>


namespace Lib
{
	Application::Application()
	{
		m_ApplicationWindow = Window::CreateWindow();
	}

	Application::~Application()
	{
	}

	void Application::Run()
	{
		std::shared_ptr<Shader> shader = Shader::CreateShader("res/shaders/basicshader.glsl");
		shader->Bind();

		std::shared_ptr<Model> model = Model::CreateModel("res/models/twocube.obj");

		TransformComponent transform;

		CameraComponent camera;

		transform.Translation.z = -3.0f;

		transform.Rotation = glm::vec3(1.0, 1.0, 1.0);
		transform.Scale = glm::vec3(0.2, 0.2, 0.2);

		shader->SetMat4("model", transform.GetTransform());
		shader->SetMat4("view", camera.SceneCamera.GetView());

		while (m_ApplicationWindow->IsOpen())
		{
			camera.SceneCamera.SetAspectRatio(m_ApplicationWindow->GetAspectRatio());
			shader->SetMat4("proj", camera.SceneCamera.GetProjection());
			m_ApplicationWindow->Clear();
			model->Draw();
			m_ApplicationWindow->OnUpdate();
		}
	}
}
