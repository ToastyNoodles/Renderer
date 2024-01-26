#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	void Load(const char* vertexFilepath, const char* fragmentFilepath);
	void LinkAttrib(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
	void Bind();
	void Unbind();

	void SetMat4(const char* name, glm::mat4 value);
	void SetVec3(const char* name, glm::vec3 value);
	uint32_t id;
};

