#pragma once
#include "../Common.h"
#include "../Renderer/Shader.h"
#include "../Core/GL.h"

class Camera
{
public:
	Camera(glm::vec3 position);
	void UploadViewProjection(Shader& shader);
	void Input(GLFWwindow* window);
	glm::vec3 position;
	glm::vec3 orientation;
	glm::vec3 up;
private:
	glm::mat4 view;
	glm::mat4 projection;
	float fov = 60.0f;
	float speed = 0.1f, sensitivity = 100.0f;
};