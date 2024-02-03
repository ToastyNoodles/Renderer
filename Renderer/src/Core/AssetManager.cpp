#include "AssetManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "../Renderer/Model.h"

namespace AssetManager
{
	std::vector<Texture> textures;
	std::vector<Model> models;
	std::vector<Material> materials;
}

uint32_t TextureFromFile(const std::string& filepath);

void AssetManager::LoadAssets()
{
	static auto textureFiles = std::filesystem::directory_iterator("res/textures/");
	static auto modelFiles = std::filesystem::directory_iterator("res/models/");

	for (const auto& file : textureFiles)
	{
		const std::filesystem::path filepath = file.path();
		FileInfo fileInfo = GetFileInfo(filepath);
		if (fileInfo.extension == "png" || fileInfo.extension == "jpg" || fileInfo.extension == "bmp")
		{
			Texture& texture = textures.emplace_back();
			texture.id = TextureFromFile(filepath.string());
			texture.info = fileInfo;
		}	
	}

	for (const auto& file : modelFiles)
	{
		const std::filesystem::path filepath = file.path();
		FileInfo fileInfo = GetFileInfo(filepath);
		if (fileInfo.extension == "obj" || fileInfo.extension == "fbx")
		{
			Model& model = models.emplace_back();
			model.Load(fileInfo.path.c_str());
			model.info = fileInfo;
		}
	}
}

Texture* AssetManager::GetTexture(const std::string& name)
{
	for (Texture& texture : textures)
	{
		if (texture.info.name == name)
		{
			return &texture;
		}
	}

	std::cout << "Failed to find texture " << name << "\n";
	return nullptr;
}

Model* AssetManager::GetModel(const std::string& name)
{
	for (Model& model : models)
	{
		if (model.info.name == name)
		{
			return &model;
		}
	}

	std::cout << "Failed to find model " << name << "\n";
	return nullptr;
}

uint32_t TextureFromFile(const std::string& filepath)
{
	uint32_t textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format{};
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		std::cout << "Loaded Texture " << filepath << std::endl;
	}
	else
	{
		std::cout << "Texture failed to load " << filepath << std::endl;
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);
	return textureID;
}


