#include "Editor.h"
#include "../Core/Scene.h"
#include "../Renderer/Renderer.h"
#include <sstream>

namespace Editor
{
	bool showLightsWindow = false;
	bool showObjectsWindow = false;
	bool showColorAttachments = false;
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

	ImGui::BeginMainMenuBar();
	if (ImGui::MenuItem("Objects")) { showObjectsWindow = !showObjectsWindow; }
	if (ImGui::MenuItem("Lights")) { showLightsWindow = !showLightsWindow; }
	if (ImGui::MenuItem("Shadows")) { Renderer::RenderShadows = !Renderer::RenderShadows; }
	if (ImGui::MenuItem("Attachments")) { showColorAttachments = !showColorAttachments; }
	ImGui::EndMainMenuBar();

	if (showObjectsWindow)
	{
		ImGui::Begin("Objects");
		int index = 0;
		for (GameObject& gameObject : Scene::gameObjects)
		{
			if (ImGui::TreeNodeEx(std::string("Object " + std::to_string(index++) + " | " + gameObject.model->info.name).c_str()))
			{
				ImGui::Checkbox("Active", &gameObject.active);
				ImGui::DragFloat3("Position", (float*)&gameObject.transform.position, 0.1f);
				ImGui::DragFloat3("Rotation", (float*)&gameObject.transform.rotation, 0.1f);
				ImGui::DragFloat3("Scale", (float*)&gameObject.transform.scale, 0.1f);
				ImGui::Text(std::string("Albedo " + gameObject.material.albedo.info.name).c_str());
				ImGui::Text(std::string("Normal " + gameObject.material.normal.info.name).c_str());
				ImGui::Text(std::string("Roughness " + gameObject.material.roughness.info.name).c_str());
				ImGui::Text(std::string("Metallic " + gameObject.material.metallic.info.name).c_str());
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
		ImGui::End();
	}

	if (showLightsWindow)
	{
		ImGui::Begin("Global Light");
		ImGui::DragFloat3("Position", (float*)&Scene::globalLight.direction, 0.01f);
		ImGui::ColorEdit3("Color", (float*)&Scene::globalLight.color, 0.1f);
		ImGui::End();

		ImGui::Begin("Lights");
		int index = 0;
		for (PointLight& light : Scene::lights)
		{
			if (ImGui::TreeNodeEx(std::string("Light " + std::to_string(index++)).c_str()))
			{
				ImGui::DragFloat3("Position", (float*)&light.position, 0.1f);
				ImGui::ColorEdit3("Color", (float*)&light.color, 0.1f);
				ImGui::DragFloat("Strength", &light.strength, 0.1f);
				ImGui::DragFloat("Radius", &light.radius, 0.1f);
				ImGui::TreePop();
			}
		}
		ImGui::TreePop();
		ImGui::End();
	}

	if (showColorAttachments)
	{
		ImGui::Begin("Color Attachments");
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
