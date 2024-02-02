#pragma once
#include "../Common.h"
#include "../Core/Camera.h"
#include "../Core/GameObject.h"

namespace Scene
{
	inline Camera camera = Camera(glm::vec3(0.0f, 0.0f, 5.0f));
	inline std::vector<Light> lights;
	inline std::vector<GameObject> gameObjects;

	void Init();
	void Update();
	void Draw(Shader& shader);
}