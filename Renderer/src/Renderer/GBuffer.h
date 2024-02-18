#pragma once
#include "../Common.h"

class GBuffer
{
public:
	void Init(int width, int height);
	void Bind();
	void Unbind();
	void Destory();
	uint32_t GetID() { return id; }

	uint32_t albedoTexture;
	uint32_t normalTexture;
	uint32_t roughnessTexture;
	uint32_t metallicTexture;
	uint32_t positionTexture;
	uint32_t lightTexture;
	uint32_t depthTexture;
private:
	uint32_t id;
	int width, height;
};