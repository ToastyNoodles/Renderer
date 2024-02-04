#include "Editor.h"
#include "../Core/Scene.h"
#include "../Renderer/Renderer.h"
#include <sstream>

namespace Editor
{
	bool ShowDemoWindow = false;
}

void Editor::Init(GLFWwindow* window)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();
	
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void Editor::RenderEditor()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	//ImGui::DockSpaceOverViewport();

	if (ShowDemoWindow)
		ImGui::ShowDemoWindow(&ShowDemoWindow);

	ImGui::Begin("Scene");
	ImGui::Checkbox("Draw Light Objects", &Renderer::DrawLightObjects);
	ImGui::Checkbox("Draw Skybox", &Renderer::DrawSkybox);

	if (ImGui::TreeNodeEx("Lights"))
	{
		if (ImGui::TreeNodeEx(std::string("Directional Light").c_str()))
		{
			ImGui::DragFloat3("Position", (float*)&Scene::sun.direction, 0.1f);
			ImGui::ColorEdit3("Color", (float*)&Scene::sun.color);
			ImGui::TreePop();
		}

		int index = 0;
		for (PointLight& light : Scene::lights)
		{
			if (ImGui::TreeNodeEx(std::string("Point Light " + std::to_string(index++)).c_str()))
			{
				ImGui::DragFloat3("Position", (float*)&light.position, 0.1f);
				ImGui::ColorEdit3("Color", (float*)&light.color);
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNodeEx("GameObjects"))
	{
		int index = 0;
		for (GameObject& object : Scene::gameObjects)
		{
			if (ImGui::TreeNodeEx(std::string("GameObject " + std::to_string(index++) + " | " + object.model->info.name).c_str()))
			{
				ImGui::Checkbox("Active", &object.active);
				if (ImGui::TreeNodeEx("Transform"))
				{
					ImGui::DragFloat3("Position", (float*)&object.transform.position, 0.1f);
					ImGui::DragFloat3("Rotation", (float*)&object.transform.rotation, 0.1f);
					ImGui::DragFloat3("Scale", (float*)&object.transform.scale, 0.1f);
					ImGui::TreePop();
				}
				if (ImGui::TreeNodeEx("Material"))
				{
					ImGui::Text(std::string("Diffuse Texture: " + object.material.diffuse.info.name).c_str());
					ImGui::Text(std::string("Specular Texture: " + object.material.specular.info.name).c_str());
					ImGui::DragFloat("Shininess", &object.material.shininess, 1.0f, 1.0f, 512.0f);
					ImGui::TreePop();
				}
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}
	
	

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
