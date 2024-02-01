#include "Application.h"
#include "Core/GL.h"
#include "Core/Scene.h"
#include "Core/AssetManager.h"
#include "Renderer/Renderer.h"
#include "Editor/Editor.h"

void Application::Run()
{
	Init();

	while (!GL::WindowShouldClose())
	{
		GL::ProcessWindowInput();

		Scene::Update();
		Renderer::RenderFrame();
		Editor::RenderEditor();

		GL::PollEventsSwapBuffers();
	}
}

void Application::Init()
{
	GL::Init(1280, 720, "Renderer");
	Editor::Init(GL::GetWindowPtr());
	AssetManager::LoadAssets();
	Scene::Init();

	Renderer::Init();
}
