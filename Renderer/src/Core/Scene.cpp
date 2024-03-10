#include "Scene.h"
#include "AssetManager.h"

void Scene::Init()
{
	globalLight.direction = glm::vec3(-1.0, -1.0f, 1.0);
	globalLight.color = glm::vec3(1.0f, 0.9f, 0.55f);
	
	GameObject& gameObject = gameObjects.emplace_back();
	gameObject.SetModel("weapon");
	gameObject.material.albedo = *AssetManager::GetTexture("weapon_albedo");
	gameObject.material.normal = *AssetManager::GetTexture("weapon_normal");
	gameObject.material.rma = *AssetManager::GetTexture("weapon_rma");
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