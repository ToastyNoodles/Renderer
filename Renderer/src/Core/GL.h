#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace GL
{
	void Init(int width, int height, const char* title);
	void ProcessWindowInput();
	void PollEventsSwapBuffers();
	int WindowShouldClose();
	GLFWwindow* GetWindowPtr();
	int GetWindowWidth();
	int GetWindowHeight();
}