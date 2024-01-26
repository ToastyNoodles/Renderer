#pragma once
#include "../Common.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Mesh.h"

class Model
{
public:
	void Load(const char* filepath);
	void Draw(Shader& shader);
private:
	std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Mesh> meshes;
	std::vector<Texture> loadedTextures;
};