#include "Scene.h"
#include "AssetManager.h"

void Scene::Init()
{
	PointLight& red = lights.emplace_back();
	red.position = glm::vec3(-5.0f, 1.0f, 0.0f);
	red.color = glm::vec3(0.6f, 1.0f, 0.0f);
	red.speed = 1.0f;

	PointLight& green = lights.emplace_back();
	green.position = glm::vec3(0.0f, 1.0f, 0.0f);
	green.color = glm::vec3(0.0f, 1.0f, 0.80f);
	green.speed = 2.0f;

	PointLight& blue = lights.emplace_back();
	blue.position = glm::vec3(5.0f, 1.0f, 0.0f);
	blue.color = glm::vec3(1.0f, 0.0f, 0.9f);
	blue.speed = 3.0f;

	GameObject& plane = gameObjects.emplace_back();
	plane.SetModel("plane");
	plane.material.color = *AssetManager::GetTexture("wood_diffuse");
	plane.material.normal = *AssetManager::GetTexture("wood_normal");
	plane.material.rma = *AssetManager::GetTexture("wood_specular");

	GameObject& backpack = gameObjects.emplace_back();
	backpack.SetModel("backpack");
	backpack.transform.position = glm::vec3(0.0f, 5.0f, 0.0f);
	backpack.material.color = *AssetManager::GetTexture("backpack_diffuse");
	backpack.material.normal = *AssetManager::GetTexture("backpack_normal");
	backpack.material.rma = *AssetManager::GetTexture("backpack_specular");

	GameObject& cube = gameObjects.emplace_back();
	cube.SetModel("cube");
	cube.transform.position = glm::vec3(0.0f, 1.0f, -5.0f);
	cube.material.color = *AssetManager::GetTexture("container_diffuse");
	cube.material.normal = *AssetManager::GetTexture("container_normal");
	cube.material.rma = *AssetManager::GetTexture("container_specular");

	GameObject& cube2 = gameObjects.emplace_back();
	cube2.SetModel("cube");
	cube2.transform.position = glm::vec3(5.0f, 1.0f, 5.0f);
	cube2.material.color = *AssetManager::GetTexture("container_diffuse");
	cube2.material.normal = *AssetManager::GetTexture("container_normal");
	cube2.material.rma = *AssetManager::GetTexture("container_specular");

	GameObject& cube3 = gameObjects.emplace_back();
	cube3.SetModel("cube");
	cube3.transform.position = glm::vec3(-5.0f, 1.0f, 5.0f);
	cube3.material.color = *AssetManager::GetTexture("container_diffuse");
	cube3.material.normal = *AssetManager::GetTexture("container_normal");
	cube3.material.rma = *AssetManager::GetTexture("container_specular");
}

void Scene::Update(float deltaTime)
{
	camera.Input(GL::GetWindowPtr());
}
