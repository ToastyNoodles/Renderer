#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum WindowMode { WINDOWED = 0, FULLSCREEN_BORDERLESS = 1, FULLSCREEN = 2 };

namespace GL
{
	void Init(int width, int height, const char* title);
	void ProcessWindowInput();
	void PollEventsSwapBuffers();
	void ToggleWindowMode(WindowMode mode);
	int WindowShouldClose();
	GLFWwindow* GetWindowPtr();
	int GetWindowWidth();
	int GetWindowHeight();
}