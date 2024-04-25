#include "Scene.h"
#include "AssetManager.h"

void Scene::Init()
{
	//globalLight.direction = glm::vec3(-0.84, -0.28, -0.53);
	//globalLight.color = glm::vec3(1.0f);

	PointLight& red = lights.emplace_back();
	red.color = glm::vec3(1.0f, 1.0f, 1.0f);
	red.position = glm::vec3(5.0f, 5.0f, 0.0f);
	red.radius = 100.0f;

	PointLight& green = lights.emplace_back();
	green.color = glm::vec3(1.0f, 1.0f, 1.0f);
	green.position = glm::vec3(5.0f, 2.0f, 0.0f);
	green.radius = 100.0f;

	PointLight& blue = lights.emplace_back();
	blue.color = glm::vec3(1.0f, 1.0f, 1.0f);
	blue.position = glm::vec3(2.0f, 5.0f, 0.0f);
	blue.radius = 100.0f;

	PointLight& white = lights.emplace_back();
	white.color = glm::vec3(1.0f, 1.0f, 1.0f);
	white.position = glm::vec3(2.0f, 2.0f, 0.0f);
	white.radius = 100.0f;

	int rows = 7, cols = 7;
	for (float x = 0; x < cols; x++)
	{
		for (float y = 0; y < rows; y++)
		{
			GameObject& sphere = gameObjects.emplace_back();
			sphere.SetModel("sphere");
			sphere.material.albedo = glm::vec3(0.5f, 0.0f, 0.0f);
			sphere.material.roughness = (float)x / (float)cols;
			sphere.material.specular = (float)y / (float)rows;
			sphere.transform.position = glm::vec3(x, y, -3.0f);
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
		shader.SetVec3("albedo", lightObject.material.albedo);
		shader.SetFloat("specular", lightObject.material.specular);
		shader.SetFloat("roughness", lightObject.material.roughness);
		lightObject.model->Draw();
	}*/
}