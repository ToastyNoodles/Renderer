#pragma once
#include "../Common.h"

namespace AssetManager
{
	void LoadAssets();
	Texture* GetTextureByName(const char* name);
}