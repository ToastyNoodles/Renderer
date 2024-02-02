#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 uv;
	glm::vec3 tangent;
	glm::vec3 bitangent;
};

struct Transform
{
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 rotation = glm::vec3(0.0f);
	glm::vec3 scale = glm::vec3(1.0f);
	glm::mat4 GetModelMatrix() const
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, position);
		model *= glm::mat4_cast(glm::quat(rotation));
		model = glm::scale(model, scale);
		return model;
	}
};

struct Light
{
	glm::vec3 position = glm::vec3(0.0f);
	glm::vec3 color = glm::vec3(1.0f);
	float strength = 1.0f;
	float radius = 1.0f;
};

struct Material
{
	std::string name;
	uint32_t albedo;
	uint32_t normal;
	uint32_t rma;
};

struct FileInfo
{
	std::string path = "";
	std::string name = "";
	std::string extension = "";
};

inline FileInfo GetFileInfo(const std::filesystem::path& filepath)
{
	FileInfo info;
	info.path = filepath.string();
	info.name = filepath.filename().string();
	info.name = info.name.substr(0, info.name.find_last_of('.'));
	info.extension = filepath.extension().string().erase(0, 1);
	return info;
}

struct Texture
{
	uint32_t id;
	FileInfo info;
	void Bind(uint32_t textureSlot)
	{
		glActiveTexture(GL_TEXTURE0 + textureSlot);
		glBindTexture(GL_TEXTURE_2D, id);
	}
};