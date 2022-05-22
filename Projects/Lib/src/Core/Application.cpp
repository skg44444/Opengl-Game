#include "libpch.h"
#include "Application.h"
#include "Renderer/Shader.h"
#include "Renderer/Model.h"
#include "Scene/Components.h"

namespace Lib
{
	Application::Application()
	{
		m_ApplicationWindow = Window::CreateWindow();
	}

	Application::~Application()
	{
		for (ApplicationLayer* layer : m_LayerStack)
		{
			delete layer;
		}
	}

	void Application::Run()
	{
		std::shared_ptr<Shader> shader = Shader::CreateShader("res/shaders/basicshader.glsl");

		std::shared_ptr<Shader> lightShader = Shader::CreateShader("res/shaders/lightshader.glsl");

		std::shared_ptr<Model> model = Model::CreateModel("res/models/twocube.obj");

		std::shared_ptr<Model> cubemodel = Model::CreateModel("res/models/cube.obj");

		TransformComponent transform;
		TransformComponent cubeTransform;

		cubeTransform.Translation = glm::vec3(-1.0f, 1.0f, -3.0f);
		cubeTransform.Scale = glm::vec3(0.1f);

		CameraComponent camera;

		transform.Translation.z = -3.0f;
		transform.Scale = glm::vec3(0.25f);
		transform.Rotation = glm::vec3(0.0f);

		lightShader->Bind();
		lightShader->SetVec3("lightColor", glm::vec3(1.0f));
		lightShader->SetVec3("lightPos", cubeTransform.Translation);
		
		while (m_ApplicationWindow->IsOpen())
		{
			lightShader->Bind();
			camera.SceneCamera.SetAspectRatio(m_ApplicationWindow->GetAspectRatio());
			lightShader->SetMat4("proj", camera.SceneCamera.GetProjection());
			m_ApplicationWindow->Clear();

			transform.Rotation.x += 0.005f;
			transform.Rotation.y += 0.005f;

			lightShader->SetMat4("model", transform.GetTransform());
			lightShader->SetMat4("view", camera.SceneCamera.GetView());
			lightShader->SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.2f));
			model->Draw();

			shader->Bind();
			shader->SetMat4("proj", camera.SceneCamera.GetProjection());
			shader->SetMat4("model", cubeTransform.GetTransform());
			shader->SetMat4("view", camera.SceneCamera.GetView());
			shader->SetVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
			cubemodel->Draw();

			m_ApplicationWindow->OnUpdate();
		}
	}
}
