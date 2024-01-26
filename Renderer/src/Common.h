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
};

struct MeshData
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;
};