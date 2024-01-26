#include "Camera.h"

Camera::Camera(glm::vec3 position)
{
	this->position = position;
	this->orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);

	this->view = glm::mat4(1.0f);
	this->projection = glm::mat4(1.0f);
}

void Camera::UploadViewProjection(Shader& shader)
{
	view = glm::lookAt(position, position + orientation, up);
	projection = glm::perspective(glm::radians(fov), ((float)GL::GetWindowWidth() / (float)GL::GetWindowHeight()), 0.01f, 100.0f);

	shader.SetMat4("viewProjection", projection * view);
}

void Camera::Input(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		position += speed * orientation;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		position += speed * -orientation;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		position += speed * -glm::normalize(glm::cross(orientation, up));

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		position += speed * glm::normalize(glm::cross(orientation, up));

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

		double mouseX;
		double mouseY;

		glfwGetCursorPos(window, &mouseX, &mouseY);

		int width = GL::GetWindowWidth(), height = GL::GetWindowHeight();

		float rotX = sensitivity * (float)(mouseY - (height / 2)) / height;
		float rotY = sensitivity * (float)(mouseX - (width / 2)) / width;

		glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX), glm::normalize(glm::cross(orientation, up)));

		if (abs(glm::angle(newOrientation, up) - glm::radians(90.0f)) <= glm::radians(85.0f))
		{
			orientation = newOrientation;
		}

		orientation = glm::rotate(orientation, glm::radians(-rotY), up);
		glfwSetCursorPos(window, (width / 2), (height / 2));
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}
