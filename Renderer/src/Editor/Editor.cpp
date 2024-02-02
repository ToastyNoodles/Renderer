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

	ImGui::Begin("Window");
	for (uint32_t i = 0; i < Scene::gameObjects.size() - 1; i++)
	{
		const char* positionLabel = "Light Position " + i;
		const char* colorLabel = "Light Color";
		ImGui::DragFloat3(positionLabel, (float*)&Scene::lights[i].position, 0.1f);
		ImGui::ColorEdit3(colorLabel, (float*)&Scene::lights[i].color);
	}
	for (uint32_t i = 0; i < Scene::gameObjects.size(); i++)
	{
		const char* positionLabel = "GameObject Position " + i;
		ImGui::DragFloat3(positionLabel, (float*)&Scene::gameObjects[i].transform.position, 0.1f);
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
