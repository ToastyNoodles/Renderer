#pragma once
#include "../Common.h"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include "Mesh.h"

class Model
{
public:
	Model() { };
	void Load(const char* filepath);
	void Draw();
	FileInfo info;
	std::vector<Mesh> meshes;
	static void DrawCube();
private:
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
};