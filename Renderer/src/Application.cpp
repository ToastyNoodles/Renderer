#include "Application.h"
#include "Core/GL.h"
#include "Core/AssetManager.h"
#include "Renderer/Renderer.h"

void Application::Run()
{
	Init();
	AssetManager::LoadAssets();

	while (!GL::WindowShouldClose())
	{
		GL::ProcessWindowInput();

		Renderer::RenderFrame();

		GL::PollEventsSwapBuffers();
	}
}

void Application::Init()
{
	GL::Init(1280, 720, "Renderer");

	Renderer::Init();
}
