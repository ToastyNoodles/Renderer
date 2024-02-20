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
	glm::mat4 GetLightSpaceMatrix() { return lightSpaceMatrix; }
private:
	uint32_t id = 0;
	int width = 1024, height = 1024;
	glm::mat4 lightSpaceMatrix = glm::mat4(1.0f);
};