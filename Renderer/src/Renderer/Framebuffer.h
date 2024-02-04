#pragma once
#include "../Common.h"

struct Framebuffer
{
public:
	void Init(int width, int height);
	void Bind();

	uint32_t id = 0;
	uint32_t rbo = 0;
	uint32_t colorBuffer = 0;
	int width = 0, height = 0;
};