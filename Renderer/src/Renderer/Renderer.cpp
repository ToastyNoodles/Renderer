#include "Renderer.h"
#include "../Common.h"
#include "../Core/GL.h"
#include "../Core/Scene.h"
#include "../Core/GameObject.h"
#include "../Core/AssetManager.h"
#include "Shader.h"
#include "Model.h"

namespace Renderer
{
	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));
	Shader shader;
	GameObject gameObject;
}

void Renderer::Init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	shader.Load("res/shaders/lighting.vert", "res/shaders/lighting.frag");
	shader.Bind();

	gameObject.SetModel("backpack");
}

void Renderer::RenderFrame()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera.Input(GL::GetWindowPtr());

	AssetManager::GetTexture("diffuse")->Bind(0);
	shader.SetVec3("lightPosition", Scene::light.position);
	shader.SetVec3("lightColor", Scene::light.color);
	shader.SetVec3("viewPosition", camera.position);

	shader.SetMat4("model", gameObject.transform.GetModelMatrix());;
	camera.UploadViewProjection(shader);
	gameObject.model->Draw();
}