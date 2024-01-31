#include "Application.h"
#include "Core/GL.h"
#include "Core/AssetManager.h"
#include "Renderer/Renderer.h"
#include "Editor/Editor.h"

void Application::Run()
{
	Init();

	while (!GL::WindowShouldClose())
	{
		GL::ProcessWindowInput();
		Editor::RenderEditorBegin();

		Renderer::RenderFrame();
		Editor::RenderEditor();

		Editor::RenderEditorEnd();
		GL::PollEventsSwapBuffers();
	}
}

void Application::Init()
{
	GL::Init(1280, 720, "Renderer");
	Editor::Init(GL::GetWindowPtr());
	AssetManager::LoadAssets();

	Renderer::Init();
}
