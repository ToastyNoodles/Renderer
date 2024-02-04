#include "GameObject.h"
#include "AssetManager.h"

GameObject::GameObject()
{
	transform.position = glm::vec3(0.0f);
	transform.rotation = glm::vec3(0.0f);
	transform.scale = glm::vec3(1.0f);
	active = true;
}

void GameObject::SetModel(const std::string& name)
{
	model = AssetManager::GetModel(name);
}
