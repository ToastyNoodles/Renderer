#pragma once
#include "../Common.h"
#include "Shader.h"

class Mesh
{
public:
	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
	void Setup();
	void Draw(Shader& shader);
private:
	unsigned int vao = 0, vbo = 0, ebo = 0;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};