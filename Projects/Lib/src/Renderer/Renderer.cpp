#include "libpch.h"
#include "Renderer.h"
#include "Shader.h"
namespace Lib
{
	struct RendererData
	{
		std::shared_ptr<Shader> m_LightShader;
		std::shared_ptr<Shader> m_BasicShader;
		std::shared_ptr<Shader> m_TextureShader;
		std::shared_ptr<Shader> m_multiLightTextureShader;
	};

	static RendererData s_SceneData;

	void Renderer3D::Init()
	{
		s_SceneData.m_BasicShader = Shader::CreateShader("res/shaders/basicshader.glsl");
		s_SceneData.m_LightShader = Shader::CreateShader("res/shaders/lightshader.glsl");
		s_SceneData.m_TextureShader = Shader::CreateShader("res/shaders/textureshader.glsl");
		s_SceneData.m_multiLightTextureShader = Shader::CreateShader("res/shaders/multilighttextureshader.glsl");
	}

	void Renderer3D::ShutDown()
	{
	}

	void Renderer3D::BeginScene(const Camera& camera, const glm::mat4& cameraView, const std::vector<LightData>& lightDataArray)
	{
		// Initialize Light shaders
		int i = 0;
		for (auto& lightData : lightDataArray)
		{
#if 0
			s_SceneData.m_LightShader->Bind();
			s_SceneData.m_LightShader->SetVec3("lightColor", lightData.lightColor);
			s_SceneData.m_LightShader->SetVec3("lightPos", lightData.lightPos);
			

			// Initialize texture shader
			s_SceneData.m_TextureShader->Bind();
			s_SceneData.m_TextureShader->SetVec3("lightColor", lightData.lightColor);
			s_SceneData.m_TextureShader->SetVec3("lightPos", lightData.lightPos);		
#endif
			//Initialize multilighttexture shader
			s_SceneData.m_multiLightTextureShader->Bind();
			s_SceneData.m_multiLightTextureShader->SetVec3("lightpos[" + std::to_string(i) + "]", lightData.lightPos);
			s_SceneData.m_multiLightTextureShader->SetVec3("light[" + std::to_string(i) + "].diffuse", lightData.lightColor);
			s_SceneData.m_multiLightTextureShader->SetFloat("light[" + std::to_string(i) + "].ambient", lightData.ambient);
			s_SceneData.m_multiLightTextureShader->SetFloat("light[" + std::to_string(i) + "].specular", lightData.specular);
			s_SceneData.m_multiLightTextureShader->SetFloat("light[" + std::to_string(i) + "].constant", lightData.constant);
			s_SceneData.m_multiLightTextureShader->SetFloat("light[" + std::to_string(i) + "].linear", lightData.linear);
			s_SceneData.m_multiLightTextureShader->SetFloat("light[" + std::to_string(i) + "].quadratic", lightData.quadratic);

			i++;
		}

		//set view and projection
#if 0
		s_SceneData.m_LightShader->Bind();
		s_SceneData.m_LightShader->SetMat4("proj", camera.GetProjection());
		s_SceneData.m_LightShader->SetMat4("view", cameraView);
		
		s_SceneData.m_TextureShader->Bind();
		s_SceneData.m_TextureShader->SetMat4("proj", camera.GetProjection());
		s_SceneData.m_TextureShader->SetMat4("view", cameraView);
#endif		
		s_SceneData.m_multiLightTextureShader->Bind();
		s_SceneData.m_multiLightTextureShader->SetMat4("proj", camera.GetProjection());
		s_SceneData.m_multiLightTextureShader->SetMat4("view", cameraView);
		s_SceneData.m_multiLightTextureShader->SetInt("noOfLights", lightDataArray.size());

		// Initialize Basic shader
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
			//s_SceneData.m_LightShader->Bind();
			//s_SceneData.m_LightShader->SetMat4("model", transform.GetTransform());
			//s_SceneData.m_LightShader->SetVec3("objectColor", glm::vec3(0.5f, 0.5f, 0.5f));
			//model->Draw();
		}
		else if (mode == RenderMode::TEXTURE)
		{
#if 0
			s_SceneData.m_TextureShader->Bind();
			s_SceneData.m_TextureShader->SetMat4("model", transform.GetTransform());
#endif
			s_SceneData.m_multiLightTextureShader->Bind();
			s_SceneData.m_multiLightTextureShader->SetMat4("model", transform.GetTransform());
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