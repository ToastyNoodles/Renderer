#include "Scene.h"
#include "AssetManager.h"

void Scene::Init()
{
	globalLight.direction = glm::vec3(-0.73f, -0.53f, -0.42f);
	globalLight.color = glm::vec3(1.0f, 0.9f, 0.55f);

	PointLight& lamp = lights.emplace_back();
	lamp.color = glm::vec3(0.3f, 0.14f, 0.12f);
	lamp.position = glm::vec3(0.0f, 7.0f, 0.0f);
	lamp.strength = 5.0;

	for (int y = 0; y < 5; y++)
	{
		GameObject& glass = transparent.emplace_back();
		glass.SetModel("plane");
		glass.transform.position = glm::vec3(0.0f, (y * 2.0f) + 1.0f, 0.0f);
		glass.material.albedo = *AssetManager::GetTexture("glass_albedo");
		glass.material.normal = *AssetManager::GetTexture("glass_normal");
		glass.material.rma = *AssetManager::GetTexture("glass_rma");
	}

	GameObject& cube = gameObjects.emplace_back();
	cube.SetModel("cube");
	cube.transform.scale = glm::vec3(1.0f, 0.5f, 1.0f);
	cube.transform.position = glm::vec3(0.0f, 0.5f, 0.0f);
	cube.material.albedo = *AssetManager::GetTexture("foil_albedo");
	cube.material.normal = *AssetManager::GetTexture("foil_normal");
	cube.material.rma = *AssetManager::GetTexture("foil_rma");
	
	GameObject& sphere = gameObjects.emplace_back();
	sphere.SetModel("sphere");
	sphere.transform.position = glm::vec3(0.0f, 4.0f, 0.0f);
	sphere.material.albedo = *AssetManager::GetTexture("glass_albedo");
	sphere.material.normal = *AssetManager::GetTexture("glass_normal");
	sphere.material.rma = *AssetManager::GetTexture("glass_rma");

	for (int x = -1; x < 2; x++)
	{
		for (int z = -1; z < 2; z++)
		{
			GameObject& plane = gameObjects.emplace_back();
			plane.SetModel("plane");
			plane.transform.position = glm::vec3(x * 10, 0.0f, z * 10);
			plane.material.albedo = *AssetManager::GetTexture("planks_albedo");
			plane.material.normal = *AssetManager::GetTexture("planks_normal");
			plane.material.rma = *AssetManager::GetTexture("planks_rma");
		}
	}
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
