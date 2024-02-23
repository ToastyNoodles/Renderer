#include "Input.h"
#include "GL.h"

namespace Input
{
    bool keyPressed[347] = { false };
}

void KeyCallback(GLFWwindow* window, int key, int keycode, int action, int mods)
{
    if (key == GLFW_KEY_UNKNOWN) return;
    if (action == GLFW_PRESS)
        Input::keyPressed[key] = true;
    else if (action == GLFW_RELEASE)
        Input::keyPressed[key] = false;
}

void Input::Init()
{
	glfwSetKeyCallback(GL::GetWindowPtr(), KeyCallback);
}

bool Input::IsKeyPressed(int keycode)
{
    return keyPressed[keycode] == true;
}
