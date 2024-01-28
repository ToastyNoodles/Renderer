#include "Renderer.h"
#include "../Common.h"
#include "../Core/GL.h"
#include "Shader.h"
#include "Model.h"

namespace Renderer
{
	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));
	Shader color;

	Model object;
}

void Renderer::Init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	color.Load("res/shaders/color.vert", "res/shaders/color.frag");
	color.Bind();

	object.Load("res/models/suzanne.fbx");
}

void Renderer::RenderFrame()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera.Input(GL::GetWindowPtr());

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	color.SetMat4("model", model);
	camera.UploadViewProjection(color);

	object.Draw(color);
}