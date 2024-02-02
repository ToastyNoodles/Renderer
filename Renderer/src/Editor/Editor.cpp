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
	ImGui::DragFloat3("Light Position", (float*)&Scene::lights[0].position);
	ImGui::End();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
