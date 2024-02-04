#pragma once
#include "../Common.h"

struct Framebuffer
{
public:
	void Init(int width, int height);
	void Recreate(int width, int height);

	uint32_t id = 0;
	uint32_t rbo = 0;
	uint32_t colorBuffer = 0;
	int width = 0, height = 0;
};