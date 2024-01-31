#include "Renderer.h"
#include "../Common.h"
#include "../Core/GL.h"
#include "../Core/AssetManager.h"
#include "Shader.h"
#include "Model.h"

namespace Renderer
{
	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));
	Shader shader;
}

void Renderer::Init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	shader.Load("res/shaders/texture.vert", "res/shaders/texture.frag");
	shader.Bind();
}

void Renderer::RenderFrame()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera.Input(GL::GetWindowPtr());

	AssetManager::GetTexture("wall")->Bind(0);
	Transform transform = Transform();
	shader.SetMat4("model", transform.GetModelMatrix());
	camera.UploadViewProjection(shader);
}