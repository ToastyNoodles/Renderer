#include "Renderer.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../Core/GL.h"
#include "Shader.h"
#include "Model.h"

namespace Renderer
{
	Camera camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));
	Shader sColor;
	Shader sObject;
	Model monkey;
}

void Renderer::Init()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	sColor.Load("res/shaders/color.vert", "res/shaders/color.frag");
	sObject.Load("res/shaders/object.vert", "res/shaders/object.frag");

	monkey.Load("res/models/backpack/backpack.obj");

	sObject.Bind();
}

void Renderer::RenderFrame()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera.Input(GL::GetWindowPtr());

	for (int x = 0; x < 5; x++)
	{
		for (int y = 0; y < 5; y++)
		{
			for (int z = 0; z < 5; z++)
			{
				glm::mat4 model = glm::mat4(1.0f);
				model = glm::translate(model, glm::vec3(x * 5.0f, y * 5.0f, -z * 5.0f));
				sObject.SetMat4("model", model);
				camera.UploadViewProjection(sObject);
				monkey.Draw(sObject);
			}
		}
	}
}