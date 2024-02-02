#include "Editor.h"
#include "../Core/Scene.h"
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

	if (ShowDemoWindow)
		ImGui::ShowDemoWindow(&ShowDemoWindow);

	ImGui::Begin("Scene");

	for (uint32_t i = 0; i < Scene::gameObjects.size() - 1; i++)
	{
		std::string label = "Light " + std::to_string(i);
		if (ImGui::TreeNode(label.c_str()))
		{
			ImGui::DragFloat3("Position", (float*)&Scene::lights[i].position, 0.1f);
			ImGui::ColorEdit3("Color", (float*)&Scene::lights[i].color);
			ImGui::TreePop();
		}
	}

	for (uint32_t i = 0; i < Scene::gameObjects.size(); i++)
	{
		std::string label = "GameObject " + std::to_string(i);
		if (ImGui::TreeNode(label.c_str()))
		{
			ImGui::DragFloat3("Position", (float*)&Scene::gameObjects[i].transform.position, 0.1f);
			ImGui::DragFloat3("Rotation", (float*)&Scene::gameObjects[i].transform.rotation, 0.1f);
			ImGui::DragFloat3("Scale", (float*)&Scene::gameObjects[i].transform.scale, 0.1f);
			ImGui::TreePop();
		}
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
