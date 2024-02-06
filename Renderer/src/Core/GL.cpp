#include "GL.h"
#include <iostream>

void FramebufferCallback(GLFWwindow* window, int width, int height);

namespace GL
{
	GLFWwindow* window;
	GLFWmonitor* monitor;
	const GLFWvidmode* videoMode;
	int windowWidth;
	int windowHeight;
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
	glfwSetWindowPos(window, (videoMode->width / 2) - (width / 2), (videoMode->height / 2) - (height / 2));

	glfwMakeContextCurrent(window);
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetFramebufferSizeCallback(window, FramebufferCallback);
}

void GL::ProcessWindowInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window, true);
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
	GL::windowWidth = width;
	GL::windowHeight = height;
	glViewport(0, 0, width, height);
}
