#include "Renderer.h"
#include "../Common.h"
#include "../Core/GL.h"
#include "../Core/Scene.h"
#include "../Core/AssetManager.h"
#include "Shader.h"
#include "Skybox.h"
#include "GBuffer.h"
#include "ShadowMap.h"

struct Shaders
{
	Shader color;
	Shader shadows;
	Shader geometry;
	Shader lighting;
	Shader transparency;
	Shader transparencyComposite;
	Shader skybox;
	Shader depth;
	Shader screen;
} shaders;

GBuffer gbuffer;
ShadowMap shadowMap;
Skybox sky;

void RenderShadowMap();
void GeometryPass();
void LightPass();
void TransparencyPass();
void SkyboxPass();
void DrawFullscreenQuad();

void Renderer::Init()
{
	shaders.color.Load("res/shaders/color.vert", "res/shaders/color.frag");
	shaders.shadows.Load("res/shaders/shadowmap.vert", "res/shaders/shadowmap.frag");
	shaders.geometry.Load("res/shaders/geometry.vert", "res/shaders/geometry.frag");
	shaders.lighting.Load("res/shaders/lighting.vert", "res/shaders/lighting.frag");
	shaders.transparency.Load("res/shaders/transparency.vert", "res/shaders/transparency.frag");
	shaders.transparencyComposite.Load("res/shaders/transparencyComposite.vert", "res/shaders/transparencyComposite.frag");
	shaders.skybox.Load("res/shaders/skybox.vert", "res/shaders/skybox.frag");
	shaders.depth.Load("res/shaders/screen.vert", "res/shaders/depth.frag");
	shaders.screen.Load("res/shaders/screen.vert", "res/shaders/screen.frag");

	gbuffer.Init(GL::GetWindowWidth(), GL::GetWindowHeight());
	shadowMap.Init();

	std::vector<std::string> skyboxTextureFilepaths
	{
		"res/textures/skybox/right.png",
		"res/textures/skybox/left.png",
		"res/textures/skybox/top.png",
		"res/textures/skybox/bottom.png",
		"res/textures/skybox/front.png",
		"res/textures/skybox/back.png"
	};

	sky.Load(skyboxTextureFilepaths);
}

void Renderer::RenderFrame()
{
	gbuffer.Bind();
	uint32_t attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3, GL_COLOR_ATTACHMENT4, GL_COLOR_ATTACHMENT5, GL_COLOR_ATTACHMENT6 };
	glDrawBuffers(sizeof(attachments) / sizeof(uint32_t), attachments);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (RenderShadows)
		RenderShadowMap();

	GeometryPass();
	LightPass();
	SkyboxPass();
	TransparencyPass();

	gbuffer.Unbind();
	glDisable(GL_DEPTH_TEST);
	shaders.screen.Bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gbuffer.transparencyCompositeTexture);
	DrawFullscreenQuad();
}

void RenderShadowMap()
{
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	shadowMap.Clear();
	shadowMap.Bind();

	shaders.shadows.Bind();
	shaders.shadows.SetMat4("lightSpaceMatrix", shadowMap.GetLightSpaceMatrix());
	Scene::DrawScene(shaders.shadows);
	shadowMap.Unbind();
}

void GeometryPass()
{
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	gbuffer.Bind();
	//AlbedoTexture, NormalTexture, RMATexture, PositionTexture
	uint32_t attachments[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(sizeof(attachments) / sizeof(uint32_t), attachments);

	shaders.geometry.Bind();
	shaders.geometry.SetMat4("projection", Scene::camera.GetProjection());
	shaders.geometry.SetMat4("view", Scene::camera.GetView());
	Scene::DrawScene(shaders.geometry);
}

void LightPass()
{
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	//Light Texture
	glDrawBuffer(GL_COLOR_ATTACHMENT4);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gbuffer.albedoTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gbuffer.normalTexture);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gbuffer.rmaTexture);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gbuffer.positionTexture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, shadowMap.depthTexture);

	shaders.lighting.Bind();
	shaders.lighting.SetVec3("viewPos", Scene::camera.position);
	shaders.lighting.SetMat4("lightSpaceMatrix", shadowMap.GetLightSpaceMatrix());
	shaders.lighting.SetBool("toggleShadows", Renderer::RenderShadows);

	shaders.lighting.SetVec3("globalLight.direction", Scene::globalLight.direction);
	shaders.lighting.SetVec3("globalLight.color", Scene::globalLight.color);

	int i = 0;
	for (PointLight& light : Scene::lights)
	{
		std::string position = std::string("pointLights[" + std::to_string(i) + "].position");
		std::string color = std::string("pointLights[" + std::to_string(i) + "].color");
		std::string strength = std::string("pointLights[" + std::to_string(i) + "].strength");
		std::string radius = std::string("pointLights[" + std::to_string(i) + "].radius");
		shaders.lighting.SetVec3(position.c_str(), light.position);
		shaders.lighting.SetVec3(color.c_str(), light.color);
		shaders.lighting.SetFloat(strength.c_str(), light.strength);
		shaders.lighting.SetFloat(radius.c_str(), light.radius);
		i++;
	}

	DrawFullscreenQuad();
}

void TransparencyPass()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//Transparency Texture
	glDrawBuffer(GL_COLOR_ATTACHMENT5);

	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, gbuffer.positionTexture);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, gbuffer.lightTexture);

	shaders.transparency.Bind();
	shaders.transparency.SetMat4("projection", Scene::camera.GetProjection());
	shaders.transparency.SetMat4("view", Scene::camera.GetView());

	shaders.transparency.SetVec3("viewPos", Scene::camera.position);
	shaders.transparency.SetVec3("globalLight.direction", Scene::globalLight.direction);
	shaders.transparency.SetVec3("globalLight.color", Scene::globalLight.color);

	int i = 0;
	for (PointLight& light : Scene::lights)
	{
		std::string position = std::string("pointLights[" + std::to_string(i) + "].position");
		std::string color = std::string("pointLights[" + std::to_string(i) + "].color");
		std::string strength = std::string("pointLights[" + std::to_string(i) + "].strength");
		std::string radius = std::string("pointLights[" + std::to_string(i) + "].radius");
		shaders.transparency.SetVec3(position.c_str(), light.position);
		shaders.transparency.SetVec3(color.c_str(), light.color);
		shaders.transparency.SetFloat(strength.c_str(), light.strength);
		shaders.transparency.SetFloat(radius.c_str(), light.radius);
		i++;
	}

	for (GameObject& transparentObject : Scene::transparent)
	{
		shaders.transparency.SetMat4("model", transparentObject.transform.GetModelMatrix());
		transparentObject.material.albedo.Bind(0);
		transparentObject.material.normal.Bind(1);
		transparentObject.material.rma.Bind(2);
		transparentObject.model->Draw();
	}

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//TransparencyComposite Texture
	glDrawBuffer(GL_COLOR_ATTACHMENT6);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gbuffer.lightTexture);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gbuffer.transparencyTexture);

	shaders.transparencyComposite.Bind();
	DrawFullscreenQuad();
}

void SkyboxPass()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//Light Texture
	glDrawBuffer(GL_COLOR_ATTACHMENT4);

	shaders.skybox.Bind();
	glm::mat4 view = glm::mat4(glm::mat3(Scene::camera.GetView()));
	shaders.skybox.SetMat4("projection", Scene::camera.GetProjection());
	shaders.skybox.SetMat4("view", view);
	sky.Draw();

	glDepthFunc(GL_LESS);
}

void DrawFullscreenQuad()
{
	static uint32_t vao;
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