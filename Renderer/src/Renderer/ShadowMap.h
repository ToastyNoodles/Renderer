#pragma once
#include "../Common.h"

class ShadowMap
{
public:
	void Init();
	void Bind();
	void Unbind();
	void Destroy();
	void Clear();

	uint32_t depthTexture;
private:
	uint32_t id;
	int width = 1024, height = 1024;
};