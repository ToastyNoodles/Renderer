#include "Scene.h"
#include "AssetManager.h"

namespace Scene
{
	float objectDistance[8];
	uint32_t sortedObjects[8];
}

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
		glass.transform.scale = glm::vec3(1.0f, 1.0f, 1.0f);
		glass.transform.position = glm::vec3(0.0f, (y * 2.0f) + 2.0f, 0.0f);
		glass.material.albedo = *AssetManager::GetTexture("glass_albedo");
		glass.material.normal = *AssetManager::GetTexture("glass_normal");
		glass.material.rma = *AssetManager::GetTexture("glass_rma");
	}
	
	GameObject& sphere = gameObjects.emplace_back();
	sphere.SetModel("sphere");
	sphere.transform.position = glm::vec3(0.0f, 1.0f, 0.0f);
	sphere.material.albedo = *AssetManager::GetTexture("foil_albedo");
	sphere.material.normal = *AssetManager::GetTexture("foil_normal");
	sphere.material.rma = *AssetManager::GetTexture("foil_rma");

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

int compare(const void* a, const void* b)
{
	double diff = Scene::objectDistance[*(int*)b] - Scene::objectDistance[*(int*)a];
	return  (0 < diff) - (diff < 0);
}

void Scene::DrawSortedTransparency(Shader& shader)
{
	for (int i = 0; i < transparent.size(); i++)
	{
		sortedObjects[i] = i;
		objectDistance[i] = glm::length(camera.position - transparent[i].transform.position);
	}

	qsort(sortedObjects, 8, sizeof(uint32_t), compare);

	for (int i = 0; i < transparent.size(); i++)
	{
		shader.SetMat4("model", transparent[sortedObjects[i]].transform.GetModelMatrix());
		transparent[sortedObjects[i]].material.albedo.Bind(0);
		transparent[sortedObjects[i]].material.normal.Bind(1);
		transparent[sortedObjects[i]].material.rma.Bind(2);
		transparent[sortedObjects[i]].model->Draw();
	}
}
