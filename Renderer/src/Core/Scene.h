#pragma once
#include "../Common.h"
#include "../Core/Camera.h"
#include "../Core/GameObject.h"

namespace Scene
{
	inline Camera camera = Camera(glm::vec3(0.0f, 2.0f, 8.0f));
	inline DirectionalLight sun;
	inline std::vector<PointLight> lights;
	inline std::vector<GameObject> gameObjects;

	void Init();
	void Update(float deltaTime);
}