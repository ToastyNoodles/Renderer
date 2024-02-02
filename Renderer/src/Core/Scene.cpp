#include "Scene.h"
#include "AssetManager.h"

namespace Scene
{
	GameObject* test; //instead of this get an object in the scene with its name
}

void Scene::Init()
{
	Light& light = lights.emplace_back();
	light.position = glm::vec3(0.0f, 2.0f, 0.0f);
	light.color = glm::vec3(0.9f, 0.86f, 0.68f);

	GameObject& plane = gameObjects.emplace_back();
	plane.SetModel("plane");
	plane.transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
	plane.transform.scale = glm::vec3(5.0f);
	plane.material.diffuse = *AssetManager::GetTexture("wood_diff");
	plane.material.specular = *AssetManager::GetTexture("wood_spec");

	GameObject& backpack = gameObjects.emplace_back();
	backpack.SetModel("backpack");
	backpack.transform.position = glm::vec3(0.0f, 2.5f, 0.0f);
	backpack.material.diffuse = *AssetManager::GetTexture("backpack_diffuse");
	backpack.material.specular = *AssetManager::GetTexture("backpack_specular");

	test = &backpack;
}

void Scene::Update(float deltaTime)
{
	test->transform.rotation.y += (0.1f * deltaTime);
}