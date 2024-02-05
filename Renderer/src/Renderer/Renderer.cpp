#include "Renderer.h"
#include "../Common.h"
#include "../Core/GL.h"
#include "../Core/Scene.h"
#include "../Core/AssetManager.h"
#include "Shader.h"
#include "Skybox.h"
#include "GBuffer.h"

struct RenderTarget
{
	GBuffer gbuffer;
} renderTarget;

struct Shaders
{
	Shader color;
	Shader lighting;
	Shader skybox;
	Shader screen;
	Shader geometry;
} shaders;

Skybox sky;

void DrawScene(Shader& shader);
void GeometryPass();
void RenderFullscreenQuad();

void Renderer::Init()
{
	shaders.color.Load("res/shaders/color.vert", "res/shaders/color.frag");
	shaders.lighting.Load("res/shaders/lighting.vert", "res/shaders/lighting.frag");
	shaders.skybox.Load("res/shaders/skybox.vert", "res/shaders/skybox.frag");
	shaders.screen.Load("res/shaders/screen.vert", "res/shaders/screen.frag");
	shaders.geometry.Load("res/shaders/geometry.vert", "res/shaders/geometry.frag");

	renderTarget.gbuffer.Init(GL::GetWindowWidth(), GL::GetWindowHeight());

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
}

void Renderer::RenderFrame()
{
	renderTarget.gbuffer.Bind();
	uint32_t attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(sizeof(attachments) / sizeof(uint32_t), attachments);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	GeometryPass();

	shaders.screen.Bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderTarget.gbuffer.colorTexture);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glDisable(GL_DEPTH_TEST);
	RenderFullscreenQuad();
}

void DrawScene(Shader& shader)
{
	//Skybox - Make its own pass?
	glDepthMask(GL_FALSE);
	shaders.skybox.Bind();
	glm::mat4 modifiedViewMatrix = glm::mat4(glm::mat3(Scene::camera.GetView()));
	shaders.skybox.SetMat4("projection", Scene::camera.GetProjection());
	shaders.skybox.SetMat4("view", modifiedViewMatrix);
	sky.Draw();
	shaders.skybox.Bind();
	glDepthMask(GL_TRUE);

	//Render GameObjects
	shader.Bind();
	for (GameObject& gameObject : Scene::gameObjects)
	{
		if (!gameObject.active) { continue; }
		shader.SetMat4("model", gameObject.transform.GetModelMatrix());
		gameObject.material.color.Bind(0);
		gameObject.material.normal.Bind(1);
		gameObject.material.rma.Bind(2);
		gameObject.model->Draw();
	}
}

void GeometryPass()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	renderTarget.gbuffer.Bind();
	uint32_t attachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
	glDrawBuffers(sizeof(attachments) / sizeof(uint32_t), attachments);

	shaders.geometry.Bind();
	shaders.geometry.SetMat4("projection", Scene::camera.GetProjection());
	shaders.geometry.SetMat4("view", Scene::camera.GetView());
	shaders.geometry.SetVec3("viewPos", Scene::camera.position);
	DrawScene(shaders.geometry);
	renderTarget.gbuffer.Unbind();
}

void RenderFullscreenQuad()
{
	static uint32_t vao = 0;
	if (vao == 0)
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
		uint32_t vbo;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(0));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}


//
//void Renderer::RenderSkybox()
//{
//	if (DrawSkybox)
//	{
//		
//	}
//}
//
//void Renderer::RenderGeometry()
//{
//	//Update Lights Position
//	for (PointLight& light : Scene::lights)
//	{
//		light.position.y = sin(glfwGetTime() + light.speed) + 2;
//		light.position.z = sin(glfwGetTime() + light.speed) * 3;
//	}
//
//	//Light Gameobjects
//	if (DrawLightObjects)
//	{
//		shaders.color.Bind();
//		for (PointLight& light : Scene::lights)
//		{
//			GameObject lightObject;
//			lightObject.SetModel("sphere");
//			lightObject.transform.position = light.position;
//			lightObject.transform.scale = glm::vec3(0.1f);
//
//			shaders.color.SetMat4("viewProjection", Scene::camera.GetViewProjection());
//			shaders.color.SetMat4("model", lightObject.transform.GetModelMatrix());
//			shaders.color.SetVec3("color", light.color);
//
//			lightObject.model->Draw();
//		}
//	}
//
//	//Gameobjects
//	shaders.lighting.Bind();
//	for (GameObject& gameObject : Scene::gameObjects)
//	{
//		if (!gameObject.active) { continue; }
//
//		//Material Uniforms
//		gameObject.material.diffuse.Bind(0);
//		gameObject.material.specular.Bind(1);
//		shaders.lighting.SetInt("material.diffuse", 0);
//		shaders.lighting.SetInt("material.specular", 1);
//		shaders.lighting.SetFloat("material.shininess", gameObject.material.shininess);
//
//		//Directional Light Uniforms
//		shaders.lighting.SetVec3("sun.direction", Scene::sun.direction);
//		shaders.lighting.SetVec3("sun.color", Scene::sun.color);
//
//		//Point Light Uniforms
//		int i = 0;
//		for (PointLight& light : Scene::lights)
//		{
//			std::string position = std::string("pointLights[" + std::to_string(i) + "].position");
//			std::string color = std::string("pointLights[" + std::to_string(i) + "].color");
//
//			shaders.lighting.SetVec3(position.c_str(), light.position);
//			shaders.lighting.SetVec3(color.c_str(), light.color);
//			i++;
//		}
//
//		shaders.lighting.SetMat4("viewProjection", Scene::camera.GetViewProjection());
//		shaders.lighting.SetMat4("model", gameObject.transform.GetModelMatrix());
//		shaders.lighting.SetVec3("viewPosition", Scene::camera.position);
//
//		gameObject.model->Draw();
//	}
//}
