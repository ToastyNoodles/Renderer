#include "Renderer.h"
#include "../Common.h"
#include "../Core/GL.h"
#include "Shader.h"
#include "Model.h"

namespace Renderer
{
	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));
	Shader color;
	Shader object;
	Shader lighting;
}

void Renderer::Init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	color.Load("res/shaders/color.vert", "res/shaders/color.frag");
	object.Load("res/shaders/object.vert", "res/shaders/object.frag");
	lighting.Load("res/shaders/lighting.vert", "res/shaders/lighting.frag");
}

void Renderer::RenderFrame()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera.Input(GL::GetWindowPtr());
}