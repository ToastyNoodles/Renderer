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

	int index = 0;
	for (PointLight& light : Scene::lights)
	{
		if (ImGui::TreeNodeEx(std::string("Light " + std::to_string(index++)).c_str()))
		{
			ImGui::DragFloat3("Position", (float*)&light.position, 0.1f);
			ImGui::ColorEdit3("Color", (float*)&light.color);
			ImGui::DragFloat("Quadratic", &light.quadratic, 0.005f, 0.0f, 1.0f);
			ImGui::DragFloat("Linear", &light.linear, 0.005f, 0.0f, 1.0f);
			ImGui::DragFloat("Constant", &light.constant, 0.005f, 0.0f, 1.0f);
			ImGui::TreePop();
		}
	}

	index = 0;
	for (GameObject& object : Scene::gameObjects)
	{
		if (ImGui::TreeNodeEx(std::string("GameObject " + std::to_string(index++) + " | " + object.model->info.name).c_str()))
		{
			ImGui::DragFloat3("Position", (float*)&object.transform.position, 0.1f);
			ImGui::DragFloat3("Rotation", (float*)&object.transform.rotation, 0.1f);
			ImGui::DragFloat3("Scale", (float*)&object.transform.scale, 0.1f);
			ImGui::Text(std::string(object.material.diffuse.info.name).c_str());
			ImGui::Text(std::string(object.material.specular.info.name).c_str());
			ImGui::DragFloat("Shininess", &object.material.shininess, 1.0f, 1.0f, 512.0f);
			ImGui::TreePop();
		}
	}

	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
