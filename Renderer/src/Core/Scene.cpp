#include "Scene.h"
#include "AssetManager.h"

void Scene::Init()
{
	sun.direction = glm::vec3(0.0f, -1.0f, 0.0f);
	sun.color = glm::vec3(0.0f);

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
	plane.material.diffuse = *AssetManager::GetTexture("wood_diffuse");
	plane.material.specular = *AssetManager::GetTexture("wood_specular");
	plane.material.shininess = 32.0f;

	GameObject& backpack = gameObjects.emplace_back();
	backpack.SetModel("backpack");
	backpack.transform.position = glm::vec3(0.0f, -5.0f, 0.0f);
	backpack.material.diffuse = *AssetManager::GetTexture("backpack_diffuse");
	backpack.material.specular = *AssetManager::GetTexture("backpack_specular");
	backpack.material.shininess = 16.0f;

	GameObject& cube = gameObjects.emplace_back();
	cube.SetModel("cube");
	cube.transform.position = glm::vec3(0.0f, 1.0f, 5.0f);
	cube.material.diffuse = *AssetManager::GetTexture("container_diffuse");
	cube.material.specular = *AssetManager::GetTexture("container_specular");
	cube.material.shininess = 64.0f;

	GameObject& cube2 = gameObjects.emplace_back();
	cube2.SetModel("cube");
	cube2.transform.position = glm::vec3(5.0f, 1.0f, -5.0f);
	cube2.material.diffuse = *AssetManager::GetTexture("container_diffuse");
	cube2.material.specular = *AssetManager::GetTexture("container_specular");
	cube2.material.shininess = 64.0f;

	GameObject& cube3 = gameObjects.emplace_back();
	cube3.SetModel("cube");
	cube3.transform.position = glm::vec3(-5.0f, 1.0f, -5.0f);
	cube3.material.diffuse = *AssetManager::GetTexture("container_diffuse");
	cube3.material.specular = *AssetManager::GetTexture("container_specular");
	cube3.material.shininess = 64.0f;
}

void Scene::Update(float deltaTime)
{

}
