#include "Scene.h"
#include "AssetManager.h"

void Scene::Init()
{
	PointLight& red = lights.emplace_back();
	red.position = glm::vec3(-3.0f, 3.0f, 3.0f);
	red.color = glm::vec3(0.96f, 0.34f, 0.32f);

	PointLight& green = lights.emplace_back();
	green.position = glm::vec3(3.0f, 3.0f, 3.0f);
	green.color = glm::vec3(0.36f, 0.94f, 0.33f);

	PointLight& blue = lights.emplace_back();
	blue.position = glm::vec3(0.0f, 3.0f, -3.0f);
	blue.color = glm::vec3(0.34f, 0.37f, 0.97f);

	GameObject& plane = gameObjects.emplace_back();
	plane.SetModel("plane");
	plane.material.diffuse = *AssetManager::GetTexture("wood_diffuse");
	plane.material.specular = *AssetManager::GetTexture("wood_specular");
	plane.material.shininess = 32.0f;

	GameObject& backpack = gameObjects.emplace_back();
	backpack.SetModel("cube");
	backpack.transform.position = glm::vec3(0.0f, 1.0f, 0.0f);
	backpack.material.diffuse = *AssetManager::GetTexture("container_diffuse");
	backpack.material.specular = *AssetManager::GetTexture("container_specular");
	backpack.material.shininess = 16.0f;
}

void Scene::Update(float deltaTime)
{

}