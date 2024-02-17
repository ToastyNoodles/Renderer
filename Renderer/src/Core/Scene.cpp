#include "Scene.h"
#include "AssetManager.h"

void Scene::Init()
{
	sunLight.direction = glm::normalize(glm::vec3(0.5f, -1.0f, -0.5f));
	sunLight.color = glm::vec3(0.5f, 0.5f, 0.5f);

	PointLight& red = lights.emplace_back();
	red.position = glm::vec3(-5.0f, 1.0f, -5.0f);
	red.color = glm::vec3(1.0f, 0.0f, 0.0f);

	PointLight& green = lights.emplace_back();
	green.position = glm::vec3(-5.0f, 1.0f, 5.0f);
	green.color = glm::vec3(0.0f, 1.0f, 0.0f);

	PointLight& blue = lights.emplace_back();
	blue.position = glm::vec3(5.0f, 1.0f, -5.0f);
	blue.color = glm::vec3(0.0f, 0.0f, 1.0f);

	PointLight& magenta = lights.emplace_back();
	magenta.position = glm::vec3(5.0f, 1.0f, 5.0f);
	magenta.color = glm::vec3(1.0f, 1.0f, 0.0f);

	for (int x = -1; x < 2; x++)
	{
		for (int z = -1; z < 2; z++)
		{
			GameObject& plane = gameObjects.emplace_back();
			plane.SetModel("plane");
			plane.transform.position = glm::vec3(x * 10, 0.0f, z * 10);
			plane.material.color = *AssetManager::GetTexture("planks_albedo");
			plane.material.normal = *AssetManager::GetTexture("planks_normal");
			plane.material.rma = *AssetManager::GetTexture("planks_metallic");
		}
	}

	for (int x = -2; x < 3; x++)
	{
		for (int y = 1; y < 3; y++)
		{
			GameObject& sphere = gameObjects.emplace_back();
			sphere.SetModel("sphere");
			sphere.transform.position = glm::vec3(x * 4, y * 3, 0.0f);
			sphere.material.color = *AssetManager::GetTexture("metal_albedo");
			sphere.material.normal = *AssetManager::GetTexture("metal_normal");
			sphere.material.rma = *AssetManager::GetTexture("metal_metallic");
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
		gameObject.material.color.Bind(0);
		gameObject.material.normal.Bind(1);
		gameObject.material.rma.Bind(2);
		gameObject.model->Draw();
	}
}
