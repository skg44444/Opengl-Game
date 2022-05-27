#include "libpch.h"
#include "Renderer.h"
#include "Shader.h"
namespace Lib
{
	struct RendererData
	{
		std::shared_ptr<Shader> m_LightShader;
		std::shared_ptr<Shader> m_BasicShader;
	};

	static RendererData s_SceneData;

	void Renderer3D::Init()
	{
		s_SceneData.m_BasicShader = Shader::CreateShader("res/shaders/basicshader.glsl");
		s_SceneData.m_LightShader = Shader::CreateShader("res/shaders/lightshader.glsl");
	}

	void Renderer3D::ShutDown()
	{
	}

	void Renderer3D::BeginScene(const Camera& camera, const glm::mat4& cameraView, glm::vec3 lightPos)
	{
		s_SceneData.m_LightShader->Bind();
		s_SceneData.m_LightShader->SetVec3("lightColor", glm::vec3(1.0f));
		s_SceneData.m_LightShader->SetVec3("lightPos", lightPos);
		s_SceneData.m_LightShader->SetMat4("proj", camera.GetProjection());
		s_SceneData.m_LightShader->SetMat4("view", cameraView);

		s_SceneData.m_BasicShader->Bind();
		s_SceneData.m_BasicShader->SetMat4("proj", camera.GetProjection());
		s_SceneData.m_BasicShader->SetMat4("view", cameraView);
	}

	void Renderer3D::EndScene()
	{
	}

	void Renderer3D::Draw(const std::shared_ptr<Model>& model, const TransformComponent& transform, RenderMode mode)
	{
		if (mode == RenderMode::MODEL)
		{
			s_SceneData.m_LightShader->Bind();
			s_SceneData.m_LightShader->SetMat4("model", transform.GetTransform());
			s_SceneData.m_LightShader->SetVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.2f));
			model->Draw();
		}
		else if (mode == RenderMode::LIGHT)
		{
			s_SceneData.m_BasicShader->Bind();
			s_SceneData.m_BasicShader->SetMat4("model", transform.GetTransform());
			s_SceneData.m_BasicShader->SetVec3("objectColor", glm::vec3(1.0f, 1.0f, 1.0f));
			model->Draw();
		}
	}

}