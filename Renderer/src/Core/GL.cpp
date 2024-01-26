#include "GL.h"
#include <iostream>

void FramebufferCallback(GLFWwindow* window, int width, int height);

namespace GL
{
	GLFWwindow* window;
	GLFWmonitor* monitor;
	const GLFWvidmode* videoMode;
	WindowMode windowMode;
	uint32_t windowWidth;
	uint32_t windowHeight;
}

void GL::Init(int width, int height, const char* title)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	monitor = glfwGetPrimaryMonitor();
	videoMode = glfwGetVideoMode(monitor);

	glfwWindowHint(GLFW_RED_BITS, videoMode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, videoMode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, videoMode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, videoMode->refreshRate);

	windowWidth = width;
	windowHeight = height;

	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	ToggleWindowMode(WINDOWED);

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetFramebufferSizeCallback(window, FramebufferCallback);
}

void GL::ProcessWindowInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);
	
	if (glfwGetKey(window, GLFW_KEY_I))
		ToggleWindowMode(WINDOWED);
	
	if (glfwGetKey(window, GLFW_KEY_O))
		ToggleWindowMode(FULLSCREEN_BORDERLESS);
	
	if (glfwGetKey(window, GLFW_KEY_P))
		ToggleWindowMode(FULLSCREEN);
}

void GL::ToggleWindowMode(WindowMode mode)
{
	if (mode == WINDOWED)
	{
		glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
		uint32_t windowPosX = (videoMode->width / 2) - (windowWidth / 2);
		uint32_t windowPosY = (videoMode->height / 2) - (windowHeight / 2);
		glfwSetWindowMonitor(window, nullptr, windowPosX, windowPosY, windowWidth, windowHeight, GLFW_DONT_CARE);
	}
	else if (mode == FULLSCREEN_BORDERLESS)
	{
		glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_FALSE);
		glfwSetWindowMonitor(window, nullptr, 0, 0, videoMode->width, videoMode->height, GLFW_DONT_CARE);
	}
	else if (mode == FULLSCREEN)
	{
		glfwSetWindowAttrib(window, GLFW_DECORATED, GLFW_TRUE);
		glfwSetWindowMonitor(window, monitor, 0, 0, videoMode->width, videoMode->height, videoMode->refreshRate);
	}
}

void GL::PollEventsSwapBuffers()
{
	glfwPollEvents();
	glfwSwapBuffers(window);
}

int GL::WindowShouldClose()
{
	return glfwWindowShouldClose(window);
}

GLFWwindow* GL::GetWindowPtr()
{
	return window;
}

int GL::GetWindowWidth()
{
	return windowWidth;
}

int GL::GetWindowHeight()
{
	return windowHeight;
}

void FramebufferCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
