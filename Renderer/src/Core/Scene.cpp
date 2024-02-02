#include "Scene.h"
#include "AssetManager.h"

void Scene::Init()
{
	Light light;
	light.position = glm::vec3(0.0f, 3.0f, 0.0f);
	light.color = glm::vec3(0.96f, 0.96f, 0.82f);
	lights.push_back(light);

	GameObject left;
	left.SetModel("plane");
	left.transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
	left.transform.scale = glm::vec3(2.0f);
	gameObjects.emplace_back(left);

	GameObject right;
	right.SetModel("cube");
	right.transform.position = glm::vec3(4.0f, 0.0f, 0.0f);
	gameObjects.emplace_back(right);
}

void Scene::Update()
{
	//for changing data in the scene before rendering. player positon, camera rotation. etc
}