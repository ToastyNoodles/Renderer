#include "Scene.h"
#include "AssetManager.h"

void Scene::Init()
{
	sunLight.direction = glm::normalize(glm::vec3(-0.0f, -1.0f, 0.0f));
	//sunLight.color = glm::vec3(1.0f, 0.94f, 0.67f);
	sunLight.color = glm::vec3(0.5f, 0.5f, 0.5f);

	PointLight& red = lights.emplace_back();
	red.position = glm::vec3(-10.0f, 1.0f, 0.0f);
	red.color = glm::vec3(1.0f, 0.0f, 0.0f);

	PointLight& green = lights.emplace_back();
	green.position = glm::vec3(0.0f, 1.0f, 0.0f);
	green.color = glm::vec3(0.0f, 1.0f, 0.0f);

	PointLight& blue = lights.emplace_back();
	blue.position = glm::vec3(10.0f, 1.0f, 0.0f);
	blue.color = glm::vec3(0.0f, 0.0f, 1.0f);

	PointLight& white = lights.emplace_back();
	white.position = glm::vec3(0.0f, 1.0f, 0.0f);
	white.color = glm::vec3(1.0f, 1.0f, 1.0f);

	for (int x = -1; x < 2; x++)
	{
		for (int y = -1; y < 2; y++)
		{
			GameObject& plane = gameObjects.emplace_back();
			plane.SetModel("plane");
			plane.transform.position = glm::vec3(x * 20, 0, y * 20);
			plane.material.color = *AssetManager::GetTexture("wood_diffuse");
			plane.material.normal = *AssetManager::GetTexture("wood_normal");
			plane.material.rma = *AssetManager::GetTexture("wood_specular");
		}
	}

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

	lights[3].position.x = sin(glfwGetTime()) * 20;
	lights[3].position.z = cos(glfwGetTime()) * 20;
}
