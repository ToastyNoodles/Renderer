#include "Renderer.h"
#include "../Common.h"
#include "../Core/GL.h"
#include "../Core/Scene.h"
#include "../Core/AssetManager.h"
#include "Shader.h"
#include "Skybox.h"
#include "Framebuffer.h"

namespace Renderer
{
	Shader color;
	Shader lighting;
	Shader skybox;
	Shader screen;

	Skybox sky;

	Framebuffer framebuffer;
	FullscreenQuad fullscreenQuad;

	void RenderSkybox();
	void RenderGeometry();
	void RenderFullscreenQuad();
}

void Renderer::Init()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	color.Load("res/shaders/color.vert", "res/shaders/color.frag");
	lighting.Load("res/shaders/lighting.vert", "res/shaders/lighting.frag");
	skybox.Load("res/shaders/skybox.vert", "res/shaders/skybox.frag");
	screen.Load("res/shaders/screen.vert", "res/shaders/screen.frag");

	std::vector<std::string> skyboxTextureFilepaths
	{
		"res/textures/space/right.png",
		"res/textures/space/left.png",
		"res/textures/space/top.png",
		"res/textures/space/bottom.png",
		"res/textures/space/front.png",
		"res/textures/space/back.png"
	};

	sky.Load(skyboxTextureFilepaths);
	framebuffer.Init(GL::GetWindowWidth(), GL::GetWindowHeight());
}

void Renderer::RenderFrame()
{
	Scene::camera.Input(GL::GetWindowPtr()); //This should be anywhere else

	framebuffer.Recreate(GL::GetWindowWidth(), GL::GetWindowHeight());

	//First Pass
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer.id);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	RenderSkybox();
	RenderGeometry();

	//Second Pass
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	RenderFullscreenQuad();
}

void Renderer::RenderSkybox()
{
	if (DrawSkybox)
	{
		glDepthMask(GL_FALSE);
		skybox.Bind();
		glm::mat4 modifiedViewMatrix = glm::mat4(glm::mat3(Scene::camera.GetView()));
		skybox.SetMat4("viewProjection", Scene::camera.GetProjection() * modifiedViewMatrix);
		sky.Draw();
		glDepthMask(GL_TRUE);
	}
}

void Renderer::RenderGeometry()
{
	//Update Lights Position
	for (PointLight& light : Scene::lights)
	{
		light.position.y = sin(glfwGetTime() + light.speed) + 2;
		light.position.z = sin(glfwGetTime() + light.speed) * 3;
	}

	//Light Gameobjects
	if (DrawLightObjects)
	{
		color.Bind();
		for (PointLight& light : Scene::lights)
		{
			GameObject lightObject;
			lightObject.SetModel("sphere");
			lightObject.transform.position = light.position;
			lightObject.transform.scale = glm::vec3(0.1f);

			color.SetMat4("viewProjection", Scene::camera.GetViewProjection());
			color.SetMat4("model", lightObject.transform.GetModelMatrix());
			color.SetVec3("color", light.color);

			lightObject.model->Draw();
		}
	}

	//Gameobjects
	lighting.Bind();
	for (GameObject& gameObject : Scene::gameObjects)
	{
		if (!gameObject.active) { continue; }

		//Material Uniforms
		gameObject.material.diffuse.Bind(0);
		gameObject.material.specular.Bind(1);
		lighting.SetInt("material.diffuse", 0);
		lighting.SetInt("material.specular", 1);
		lighting.SetFloat("material.shininess", gameObject.material.shininess);

		//Directional Light Uniforms
		lighting.SetVec3("sun.direction", Scene::sun.direction);
		lighting.SetVec3("sun.color", Scene::sun.color);

		//Point Light Uniforms
		int i = 0;
		for (PointLight& light : Scene::lights)
		{
			std::string position = std::string("pointLights[" + std::to_string(i) + "].position");
			std::string color = std::string("pointLights[" + std::to_string(i) + "].color");

			lighting.SetVec3(position.c_str(), light.position);
			lighting.SetVec3(color.c_str(), light.color);
			i++;
		}

		lighting.SetMat4("viewProjection", Scene::camera.GetViewProjection());
		lighting.SetMat4("model", gameObject.transform.GetModelMatrix());
		lighting.SetVec3("viewPosition", Scene::camera.position);

		gameObject.model->Draw();
	}
}

void Renderer::RenderFullscreenQuad()
{
	if (fullscreenQuad.vao == 0)
	{
		float quadVertices[] =
		{
			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 1.0f, 1.0f,
			 1.0f,  1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f
		};

		glGenVertexArrays(1, &fullscreenQuad.vao);
		glBindVertexArray(fullscreenQuad.vao);

		glGenBuffers(1, &fullscreenQuad.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, fullscreenQuad.vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	screen.Bind();
	glBindVertexArray(fullscreenQuad.vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, framebuffer.colorBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
