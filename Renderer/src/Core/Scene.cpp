#include "Scene.h"
#include "AssetManager.h"

namespace Scene
{
	GameObject visualLight;
}

void Scene::Init()
{
	Light light;
	light.position = glm::vec3(-4.0f, 5.0f, -4.0f);
	light.color = glm::vec3(0.96f, 0.96f, 0.82f);
	lights.push_back(light);

	GameObject left;
	left.SetModel("cube");
	left.transform.position = glm::vec3(0.0f, 0.0f, 0.0f);
	gameObjects.emplace_back(left);

	GameObject right;
	right.SetModel("cube");
	right.transform.position = glm::vec3(3.0f, 0.0f, 0.0f);
	gameObjects.emplace_back(right);

	visualLight.SetModel("cube");
	visualLight.transform.position = light.position;
	visualLight.transform.scale = glm::vec3(0.4f);
	gameObjects.emplace_back(visualLight);
}

void Scene::Update()
{
	//for changing data in the scene before rendering. player positon, camera rotation. etc
	visualLight.transform.position = lights[0].position;
}

void Scene::Draw(Shader& shader)
{
	for (GameObject& gameObject : gameObjects)
	{
		camera.UploadViewProjection(shader);
		shader.SetMat4("model", gameObject.transform.GetModelMatrix());
		shader.SetVec3("viewPosition", camera.position);

		for (Light& light : lights)
		{
			shader.SetVec3("lightPosition", light.position);
			shader.SetVec3("lightColor", light.color);
		}

		gameObject.model->Draw();
	}
}
