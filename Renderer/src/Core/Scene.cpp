#include "Scene.h"
#include "AssetManager.h"

void Scene::Init()
{
	Light& light = lights.emplace_back();
	light.position = glm::vec3(2.0f, 2.0f, 2.0f);
	light.color = glm::vec3(0.87f, 0.78f, 0.56f);
	light.strength = 0.1f;
	light.radius = 0.01f;
	light.intensity = 0.850f;

	GameObject& plane = gameObjects.emplace_back();
	plane.SetModel("plane");
	plane.material.diffuse = *AssetManager::GetTexture("wood_diffuse");
	plane.material.specular = *AssetManager::GetTexture("wood_specular");

	GameObject& backpack = gameObjects.emplace_back();
	backpack.SetModel("cube");
	backpack.transform.position = glm::vec3(0.0f, 1.0f, 0.0f);
	backpack.material.diffuse = *AssetManager::GetTexture("container_diffuse");
	backpack.material.specular = *AssetManager::GetTexture("container_specular");
}

void Scene::Update(float deltaTime)
{

}