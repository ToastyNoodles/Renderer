#include "Renderer.h"
#include "../Common.h"
#include "../Core/GL.h"
#include "../Core/Scene.h"
#include "../Core/AssetManager.h"
#include "Shader.h"
#include "Skybox.h"

namespace Renderer
{
	Shader color;
	Shader lighting;
	Shader object;
	Shader texture;
	Shader skybox;

	Skybox sky;
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
	skybox.Load("res/shaders/skybox.vert", "res/shaders/skybox.frag");

	std::vector<std::string> skyboxTextureFilepaths
	{
		"res/textures/skybox/right.jpg",
		"res/textures/skybox/left.jpg",
		"res/textures/skybox/top.jpg",
		"res/textures/skybox/bottom.jpg",
		"res/textures/skybox/front.jpg",
		"res/textures/skybox/back.jpg"
	};

	sky.Load(skyboxTextureFilepaths);
}

void Renderer::RenderFrame()
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Scene::camera.Input(GL::GetWindowPtr());

	//Skybox
	glDepthMask(GL_FALSE);
	skybox.Bind();
	glm::mat4 modifiedViewMatrix = glm::mat4(glm::mat3(Scene::camera.GetView()));
	skybox.SetMat4("viewProjection", Scene::camera.GetProjection() * modifiedViewMatrix);
	sky.Draw();
	glDepthMask(GL_TRUE);

	//Light Gameobjects
	if (DrawLights)
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
		//Material Uniforms
		gameObject.material.diffuse.Bind(0);
		gameObject.material.specular.Bind(1);
		lighting.SetInt("material.diffuse", 0);
		lighting.SetInt("material.specular", 1);
		lighting.SetFloat("material.shininess", gameObject.material.shininess);
		
		//Point Light Uniforms
		int i = 0;
		for (PointLight& light : Scene::lights)
		{
			std::string position = std::string("pointLights[" + std::to_string(i) + "].position");
			std::string color = std::string("pointLights[" + std::to_string(i) + "].color");
			std::string quadratic = std::string("pointLights[" + std::to_string(i) + "].quadratic");
			std::string linear = std::string("pointLights[" + std::to_string(i) + "].linear");
			std::string constant = std::string("pointLights[" + std::to_string(i) + "].constant");

			lighting.SetVec3(position.c_str(), light.position);
			lighting.SetVec3(color.c_str(), light.color);
			lighting.SetFloat(quadratic.c_str(), light.quadratic);
			lighting.SetFloat(linear.c_str(), light.linear);
			lighting.SetFloat(constant.c_str(), light.constant);
			i++;
		}

		lighting.SetMat4("viewProjection", Scene::camera.GetViewProjection());
		lighting.SetMat4("model", gameObject.transform.GetModelMatrix());
		lighting.SetVec3("viewPosition", Scene::camera.position);

		gameObject.model->Draw();
	}
}