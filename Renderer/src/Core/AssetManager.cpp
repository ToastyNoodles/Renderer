#include "AssetManager.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace AssetManager
{
	std::vector<Texture> textures;
}

uint32_t TextureFromFile(const std::string& filepath);

void AssetManager::LoadAssets()
{
	static auto textureFiles = std::filesystem::directory_iterator("res/textures/");

	for (const auto& file : textureFiles)
	{
		const std::filesystem::path filepath = file.path();
		FileInfo fileInfo = GetFileInfo(filepath);
		if (fileInfo.extension == "png" || fileInfo.extension == "jpg" || fileInfo.extension == "bmp")
		{
			Texture texture;
			texture.id = TextureFromFile(filepath.string());
			texture.info = fileInfo;
			textures.push_back(texture);
		}	
	}
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
	return textureID;
}


