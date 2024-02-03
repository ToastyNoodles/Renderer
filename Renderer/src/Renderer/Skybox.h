#pragma once
#include "../Common.h"

struct Skybox
{
	void Load(const std::vector<std::string> skyboxTextureFilepaths);
	void Draw();

	uint32_t id = 0;
	uint32_t vao = 0, vbo = 0;
};