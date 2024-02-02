#include "Renderer.h"
#include "../Common.h"
#include "../Core/GL.h"
#include "../Core/Scene.h"
#include "../Core/AssetManager.h"
#include "Shader.h"

namespace Renderer
{
	Shader color;
	Shader lighting;
	Shader object;
	Shader texture;
}

void Renderer::Init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	color.Load("res/shaders/color.vert", "res/shaders/color.frag");
	lighting.Load("res/shaders/lighting.vert", "res/shaders/lighting.frag");
	object.Load("res/shaders/object.vert", "res/shaders/object.frag");
	texture.Load("res/shaders/texture.vert", "res/shaders/texture.frag");
}

void Renderer::RenderFrame()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Scene::camera.Input(GL::GetWindowPtr());

	//Draw light objects
	object.Bind();
	for (Light& light : Scene::lights)
	{
		GameObject lightObject;
		lightObject.SetModel("cube");
		lightObject.transform.position = light.position;
		lightObject.transform.scale = glm::vec3(0.1f);

		Scene::camera.UploadViewProjection(object);
		object.SetMat4("model", lightObject.transform.GetModelMatrix());

		lightObject.model->Draw();
	}

	//Draw gameobjects
	lighting.Bind();
	AssetManager::GetTexture("wood_diff")->Bind(0);
	AssetManager::GetTexture("wood_spec")->Bind(1);
	for (GameObject& gameObject : Scene::gameObjects)
	{
		Scene::camera.UploadViewProjection(lighting);
		lighting.SetMat4("model", gameObject.transform.GetModelMatrix());
		lighting.SetVec3("viewPosition", Scene::camera.position);

		for (Light& light : Scene::lights)
		{
			lighting.SetVec3("lightPosition", light.position);
			lighting.SetVec3("lightColor", light.color);
		}

		gameObject.model->Draw();
	}
}