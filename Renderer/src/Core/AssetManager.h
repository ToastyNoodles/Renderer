#pragma once
#include "../Common.h"
#include "../Renderer/Model.h"

namespace AssetManager
{
	void LoadAssets();
	Texture* GetTexture(const std::string& name);
	Model* GetModel(const std::string& name);
}