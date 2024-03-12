#include "Scene.h"
#include "AssetManager.h"

void Scene::Init()
{
	globalLight.direction = glm::vec3(-0.84, -0.08, -0.53);
	globalLight.color = glm::vec3(1.0f);

	//PointLight light = lights.emplace_back();

	GameObject& weapon1 = gameObjects.emplace_back();
	weapon1.SetModel("m16");
	weapon1.material.albedo = *AssetManager::GetTexture("m16_albedo");
	weapon1.material.normal = *AssetManager::GetTexture("m16_normal");
	weapon1.material.rma = *AssetManager::GetTexture("m16_rma");

	GameObject& weapon2 = gameObjects.emplace_back();
	weapon2.active = false;
	weapon2.SetModel("m9");
	weapon2.material.albedo = *AssetManager::GetTexture("m9_albedo");
	weapon2.material.normal = *AssetManager::GetTexture("m9_normal");
	weapon2.material.rma = *AssetManager::GetTexture("m9_rma");
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