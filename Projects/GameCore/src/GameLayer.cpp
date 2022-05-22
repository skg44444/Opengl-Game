#include "GameLayer.h"

GameLayer::GameLayer()
{
	shader = Lib::Shader::CreateShader("res/shaders/basicshader.glsl");

	lightShader = Lib::Shader::CreateShader("res/shaders/lightshader.glsl");

	model = Lib::Model::CreateModel("res/models/twocube.obj");

	cubemodel = Lib::Model::CreateModel("res/models/cube.obj");

	cubeTransform.Translation = glm::vec3(-1.0f, 1.0f, -3.0f);
	cubeTransform.Scale = glm::vec3(0.1f);

	transform.Translation.z = -3.0f;
	transform.Scale = glm::vec3(0.25f);
	transform.Rotation = glm::vec3(0.0f);

	lightShader->Bind();
	lightShader->SetVec3("lightColor", glm::vec3(1.0f));
	lightShader->SetVec3("lightPos", cubeTransform.Translation);
}

GameLayer::~GameLayer()
{
}

void GameLayer::OnUpdate(float dt, float AspectRatio)
{
	lightShader->Bind();
	camera.SceneCamera.SetAspectRatio(AspectRatio);
	lightShader->SetMat4("proj", camera.SceneCamera.GetProjection());

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
}
