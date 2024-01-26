#include "Application.h"
#include "Core/GL.h"
#include "Renderer/Renderer.h"

void Application::Run()
{
	Init();

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
