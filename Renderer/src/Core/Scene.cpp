#include "Scene.h"
#include "AssetManager.h"

void Scene::Init()
{
	PointLight& red = lights.emplace_back();
	red.position = glm::vec3(-8.0f, 1.0f, -5.0f);
	red.color = glm::vec3(1.0f, 0.0f, 0.0f);

	PointLight& green = lights.emplace_back();
	green.position = glm::vec3(0.0f, 1.0f, -5.0f);
	green.color = glm::vec3(0.0f, 1.0f, 0.0f);

	PointLight& blue = lights.emplace_back();
	blue.position = glm::vec3(8.0f, 1.0f, -5.0f);
	blue.color = glm::vec3(0.0f, 0.0f, 1.0f);

	PointLight& yellow = lights.emplace_back();
	yellow.position = glm::vec3(-8.0f, 1.0f, 5.0f);
	yellow.color = glm::vec3(1.0f, 1.0f, 0.0f);

	PointLight& cyan = lights.emplace_back();
	cyan.position = glm::vec3(0.0f, 1.0f, 5.0f);
	cyan.color = glm::vec3(0.0f, 1.0f, 1.0f);

	PointLight& magenta = lights.emplace_back();
	magenta.position = glm::vec3(8.0f, 1.0f, 5.0f);
	magenta.color = glm::vec3(1.0f, 0.0f, 1.0f);

	for (int x = -4; x < 5; x++)
	{
		for (int z = -4; z < 5; z++)
		{
			GameObject& plane = gameObjects.emplace_back();
			plane.SetModel("plane");
			plane.transform.position = glm::vec3(x * 10, 0.0f, z * 10);
			plane.material.albedo = *AssetManager::GetTexture("planks_albedo");
			plane.material.normal = *AssetManager::GetTexture("planks_normal");
			plane.material.roughness = *AssetManager::GetTexture("planks_roughness");
			plane.material.metallic = *AssetManager::GetTexture("planks_metallic");
		}
	}

	for (int x = -2; x < 3; x++)
	{
		for (int y = 1; y < 3; y++)
		{
			GameObject& sphere = gameObjects.emplace_back();
			sphere.SetModel("sphere");
			sphere.transform.position = glm::vec3(x * 4, y * 3, 0.0f);
			sphere.material.albedo = *AssetManager::GetTexture("foil_albedo");
			sphere.material.normal = *AssetManager::GetTexture("foil_normal");
			sphere.material.roughness = *AssetManager::GetTexture("foil_roughness");
			sphere.material.metallic = *AssetManager::GetTexture("foil_metallic");
		}
	}
}

void Scene::Update(float deltaTime)
{
	camera.Input(GL::GetWindowPtr());
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
		gameObject.material.roughness.Bind(2);
		gameObject.material.metallic.Bind(3);
		gameObject.model->Draw();
	}
}
