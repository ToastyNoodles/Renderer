#include "Renderer.h"
#include "../Common.h"
#include "../Core/GL.h"
#include "../Core/Scene.h"
#include "../Core/AssetManager.h"
#include "Shader.h"

namespace Renderer
{
	Shader shader;
}

void Renderer::Init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	shader.Load("res/shaders/lighting.vert", "res/shaders/lighting.frag");
	shader.Bind();
}

void Renderer::RenderFrame()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Scene::camera.Input(GL::GetWindowPtr());

	AssetManager::GetTexture("container_diffuse")->Bind(0);
	AssetManager::GetTexture("container_specular")->Bind(1);
	Scene::Draw(shader);
}