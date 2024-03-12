#pragma once
#include "../Common.h"
#include "../Renderer/Shader.h"
#include "../Core/GL.h"

class Camera
{
public:
	Camera(glm::vec3 position);
	glm::mat4 GetView();
	glm::mat4 GetProjection();
	void Input(GLFWwindow* window);
	glm::vec3 position;
	glm::vec3 orientation;
	glm::vec3 up;
private:
	glm::mat4 view;
	glm::mat4 projection;
	float fov = 60.0f, near = 0.01f, far = 1000.0f;
	float speed = 0.02f, sensitivity = 100.0f;
};