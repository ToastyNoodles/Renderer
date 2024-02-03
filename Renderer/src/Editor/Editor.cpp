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
	ImGui::Checkbox("Draw Lights", &Renderer::DrawLights);
	for (uint32_t i = 0; i < Scene::gameObjects.size(); i++)
	{
		std::string label = "Light " + std::to_string(i);
		if (ImGui::TreeNodeEx(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::DragFloat3("Position", (float*)&Scene::lights[i].position, 0.1f);
			ImGui::ColorEdit3("Color", (float*)&Scene::lights[i].color);
			ImGui::DragFloat("Strength", (float*)&Scene::lights[i].strength, 0.002f, 0.0f, 1.0f);
			ImGui::DragFloat("Radius", (float*)&Scene::lights[i].radius, 0.002f, 0.0f, 1.0f);
			ImGui::DragFloat("Intensity", (float*)&Scene::lights[i].intensity, 0.002f, 0.0f, 1.0f);
			ImGui::TreePop();
		}
	}

	std::string textureLabel = "";
	for (GameObject& gameObject : Scene::gameObjects)
	{
		if (ImGui::TreeNodeEx(gameObject.model->info.name.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::DragFloat3("Position", (float*)&gameObject.transform.position, 0.1f);
			ImGui::DragFloat3("Rotation", (float*)&gameObject.transform.rotation, 0.1f);
			ImGui::DragFloat3("Scale", (float*)&gameObject.transform.scale, 0.1f);
			textureLabel = "Diffuse: " + gameObject.material.diffuse.info.name;
			ImGui::Text(textureLabel.c_str());
			textureLabel = "Specular: " + gameObject.material.specular.info.name;
			ImGui::Text(textureLabel.c_str());
			ImGui::TreePop();
		}
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
