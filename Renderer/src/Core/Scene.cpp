#include "Scene.h"
#include "AssetManager.h"

void Scene::Init()
{
	globalLight.direction = glm::vec3(-0.84, -0.08, -0.53);
	globalLight.color = glm::vec3(1.0f);

	//PointLight light = lights.emplace_back();

	GameObject& m9 = gameObjects.emplace_back();
	m9.SetModel("m9");
	m9.material.albedo = *AssetManager::GetTexture("m9_albedo");
	m9.material.normal = *AssetManager::GetTexture("m9_normal");
	m9.material.rma = *AssetManager::GetTexture("m9_rma");
}

void Scene::Update(float deltaTime)
{
	camera.Input(GL::GetWindowPtr());
	globalLight.direction = glm::normalize(globalLight.direction);
}

void Scene::DrawScene(Shader& shader)
{
	shader.Bind();
	for (GameObject& gameObject : gameObjects)
	{
		if (!gameObject.active) { continue; }
		shader.SetMat4("model", gameObject.transform.GetModelMatrix());
		gameObject.material.albedo.Bind(0);
		gameObject.material.normal.Bind(1);
		gameObject.material.rma.Bind(2);
		gameObject.model->Draw();
	}
}