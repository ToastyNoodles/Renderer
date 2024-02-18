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

	if (ImGui::TreeNodeEx("Lights"))
	{
		int index = 0;
		for (PointLight& light : Scene::lights)
		{
			if (ImGui::TreeNodeEx(std::string("Light " + std::to_string(index++)).c_str()))
			{
				ImGui::DragFloat3("Position", (float*)&light.position, 0.1f);
				ImGui::ColorEdit3("Color", (float*)&light.color, 0.1f);
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
				ImGui::DragFloat3("Position", (float*)&object.transform.position, 0.1f);
				ImGui::DragFloat3("Rotation", (float*)&object.transform.rotation, 0.1f);
				ImGui::DragFloat3("Scale", (float*)&object.transform.scale, 0.1f);
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
