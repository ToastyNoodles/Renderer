#pragma once
#include "../Common.h"
#include "../Core/Camera.h"
#include "../Core/GameObject.h"

namespace Scene
{
	inline Camera camera = Camera(glm::vec3(0.0f, 1.0f, 10.0f));
	inline std::vector<GameObject> gameObjects;
	inline std::vector<PointLight> lights;
	inline GlobalLight globalLight;
	inline float biasMax = 0.020f, biasMin = 0.010f;

	void Init();
	void Update(float deltaTime);
	void DrawScene(Shader& shader);
}