#include "Scene.h"
#include "AssetManager.h"

void Scene::Init()
{
	globalLight.direction = glm::vec3(-0.24f, -0.66, -0.71);
	globalLight.color = glm::vec3(1.0f);

	/*
	PointLight& red = lights.emplace_back();
	red.color = glm::vec3(1.0f, 0.0f, 0.0f);
	red.position = glm::vec3(-3.0f, 0.0f, 2.0f);

	PointLight& green = lights.emplace_back();
	green.color = glm::vec3(0.0f, 1.0f, 0.0f);
	green.position = glm::vec3(0.0f, 0.0f, 2.0f);

	PointLight& blue = lights.emplace_back();
	blue.color = glm::vec3(0.0f, 0.0f, 1.0f);
	blue.position = glm::vec3(3.0f, 0.0f, 2.0f);
	*/

	//int rows = 7, cols = 7;
	//for (float x = 0; x < cols; x++)
	//{
	//	for (float y = 0; y < rows; y++)
	//	{
	//		GameObject& sphere = gameObjects.emplace_back();
	//		sphere.SetModel("sphere");
	//		sphere.transform.position = glm::vec3(x * 1.2, y * 1.2, -3.0f);
	//		sphere.material.roughness = (float)x / (float)cols;
	//		sphere.material.specular = (float)y / (float)rows;
	//	}
	//}

	GameObject& weapon = gameObjects.emplace_back();
	weapon.SetModel("gun");
	weapon.transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
	weapon.material.albedoMap = *AssetManager::GetTexture("gun_albedo");
	weapon.material.normalMap = *AssetManager::GetTexture("gun_normal");
	weapon.material.rmaMap = *AssetManager::GetTexture("gun_rma");
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
		shader.SetVec3("albedo", gameObject.material.albedo);
		shader.SetFloat("specular", gameObject.material.specular);
		shader.SetFloat("roughness", gameObject.material.roughness);
		gameObject.material.albedoMap.Bind(0);
		gameObject.material.normalMap.Bind(1);
		gameObject.material.rmaMap.Bind(2);
		gameObject.model->Draw();
	}

	//For visualizing pointlights
	/*for (PointLight& light : lights)
	{
		GameObject lightObject;
		lightObject.SetModel("sphere");
		lightObject.material.albedo = light.color;
		lightObject.transform.position = light.position;
		lightObject.transform.scale = glm::vec3(0.1f);
		shader.SetMat4("model", lightObject.transform.GetModelMatrix());
		lightObject.model->Draw();
	}*/
}