#pragma once
#include "../Common.h"
#include "../Renderer/Model.h"

struct GameObject
{
	GameObject();
	void SetModel(const std::string& name);
	Transform transform;
	Model* model = nullptr;
	Material material;
};