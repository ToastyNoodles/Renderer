#include "Scene.h"
#include "AssetManager.h"

void Scene::Init()
{
	sun.direction = glm::vec3(-1.0f, -0.4f, -0.5f);
	sun.color = glm::vec3(0.23f, 0.23f, 0.37f);

	PointLight& red = lights.emplace_back();
	red.position = glm::vec3(-3.0f, 3.0f, 3.0f);
	red.color = glm::vec3(1.0f, 0.0f, 0.0f);

	PointLight& green = lights.emplace_back();
	green.position = glm::vec3(3.0f, 3.0f, 3.0f);
	green.color = glm::vec3(0.0f, 1.0f, 0.0f);

	PointLight& blue = lights.emplace_back();
	blue.position = glm::vec3(0.0f, 3.0f, -3.0f);
	blue.color = glm::vec3(0.0f, 0.0f, 1.0f);

	GameObject& plane = gameObjects.emplace_back();
	plane.SetModel("plane");
	plane.material.diffuse = *AssetManager::GetTexture("wood_diffuse");
	plane.material.specular = *AssetManager::GetTexture("wood_specular");
	plane.material.shininess = 32.0f;

	GameObject& backpack = gameObjects.emplace_back();
	backpack.SetModel("backpack");
	backpack.transform.position = glm::vec3(0.0f, 2.0f, 0.0f);
	backpack.material.diffuse = *AssetManager::GetTexture("backpack_diffuse");
	backpack.material.specular = *AssetManager::GetTexture("backpack_specular");
	backpack.material.shininess = 16.0f;
}

void Scene::Update(float deltaTime)
{

}