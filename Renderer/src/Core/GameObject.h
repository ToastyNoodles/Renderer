#pragma once
#include "../Common.h"
#include "../Renderer/Model.h"

struct GameObject
{
	GameObject() {};
	GameObject(const std::string& name);
	void SetModel(const std::string& name);
	Transform transform;
	Model* model = nullptr;
	std::string name;
};