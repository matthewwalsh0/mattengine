#include "Model.h"

#include "Log.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

using namespace MattEngine;

Model::Model(const std::string& file, bool flip) {
	Assimp::Importer importer;
	unsigned int flags = aiProcess_Triangulate;

	if (flip) {
		flags |= aiProcess_FlipUVs;
	}

	const aiScene* scene = importer.ReadFile(file, flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!scene->mRootNode) {
		MATTENGINE_ASSERT(false, "Failed to load model: %s", file.c_str());
	}

	std::string directory = file.substr(0, file.find_last_of('/'));
	Texture* texture = nullptr;

	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
			Vertex vertex;

			glm::vec3 position;
			position.x = mesh->mVertices[j].x;
			position.y = mesh->mVertices[j].y;
			position.z = mesh->mVertices[j].z;
			vertex.Position = position;

			glm::vec3 normal;
			normal.x = mesh->mNormals[j].x;
			normal.y = mesh->mNormals[j].y;
			normal.z = mesh->mNormals[j].z;
			vertex.Normal = normal;

			glm::vec2 texturePosition;
			texturePosition.x = mesh->mTextureCoords[0][j].x;
			texturePosition.y = mesh->mTextureCoords[0][j].y;
			vertex.TexturePosition = texturePosition;

			vertices.push_back(vertex);
		}

		for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
			for (unsigned int k = 0; k < mesh->mFaces[j].mNumIndices; k++)
				indices.push_back(mesh->mFaces[j].mIndices[k]);
		}

		if (mesh->mMaterialIndex >= 0 && !texture) {
			aiString texturePath;
			scene->mMaterials[mesh->mMaterialIndex]->GetTexture(
				aiTextureType_DIFFUSE, 0, &texturePath);

			texture =
				new Texture(directory + "/" + std::string(texturePath.C_Str()));
		}

		Meshes.push_back(Mesh(vertices, indices, texture));
	}
}
