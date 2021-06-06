#include "Model.h"

#include "Log.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

using namespace MattEngine;

namespace Utils {

static glm::mat4 toGLMMatrix(const aiMatrix4x4& from) {
	glm::mat4 to;

	to[0][0] = from.a1;
	to[1][0] = from.a2;
	to[2][0] = from.a3;
	to[3][0] = from.a4;
	to[0][1] = from.b1;
	to[1][1] = from.b2;
	to[2][1] = from.b3;
	to[3][1] = from.b4;
	to[0][2] = from.c1;
	to[1][2] = from.c2;
	to[2][2] = from.c3;
	to[3][2] = from.c4;
	to[0][3] = from.d1;
	to[1][3] = from.d2;
	to[2][3] = from.d3;
	to[3][3] = from.d4;

	return to;
}

static inline glm::vec3 toGLMVec3(const aiVector3D& from) {
	return glm::vec3(from.x, from.y, from.z);
}

static inline glm::quat toGLMQuat(const aiQuaternion& from) {
	return glm::quat(from.w, from.x, from.y, from.z);
}

static void readVertices(const aiMesh& rawMesh, std::vector<Vertex>& vertices) {
	for (unsigned int vertexIndex = 0; vertexIndex < rawMesh.mNumVertices;
		 vertexIndex++) {
		Vertex vertex;
		vertex.Position = Utils::toGLMVec3(rawMesh.mVertices[vertexIndex]);
		vertex.Normal = Utils::toGLMVec3(rawMesh.mNormals[vertexIndex]);

		for (int vertexBoneIndex = 0; vertexBoneIndex < 4; vertexBoneIndex++) {
			vertex.BoneIds[vertexBoneIndex] = -1;
			vertex.BoneWeights[vertexBoneIndex] = 0.0f;
		}

		glm::vec2 texturePosition;
		texturePosition.x = rawMesh.mTextureCoords[0][vertexIndex].x;
		texturePosition.y = rawMesh.mTextureCoords[0][vertexIndex].y;
		vertex.TexturePosition = texturePosition;

		vertices.push_back(vertex);
	}
}

static void readIndices(
	const aiMesh& rawMesh, std::vector<unsigned int>& indices) {
	for (unsigned int faceIndex = 0; faceIndex < rawMesh.mNumFaces;
		 faceIndex++) {
		for (unsigned int indexIndex = 0;
			 indexIndex < rawMesh.mFaces[faceIndex].mNumIndices; indexIndex++)
			indices.push_back(rawMesh.mFaces[faceIndex].mIndices[indexIndex]);
	}
}

static void addVertexBone(Vertex& vertex, int boneId, float boneWeight) {
	for (int vertexBoneIndex = 0; vertexBoneIndex < 4; ++vertexBoneIndex) {
		if (vertex.BoneIds[vertexBoneIndex] < 0 && boneWeight != 0.0f) {
			vertex.BoneWeights[vertexBoneIndex] = boneWeight;
			vertex.BoneIds[vertexBoneIndex] = boneId;
			break;
		}
	}
}

static void readBones(const aiMesh& rawMesh,
	std::map<std::string, Bone>& bonesByName, std::vector<Vertex>& vertices) {
	for (int boneIndex = 0; boneIndex < rawMesh.mNumBones; ++boneIndex) {
		int boneId = -1;
		std::string boneName = rawMesh.mBones[boneIndex]->mName.C_Str();

		if (bonesByName.find(boneName) == bonesByName.end()) {
			Bone bone;
			bone.Id = bonesByName.size();
			bone.Name = boneName;
			bone.ModelToLocalTransform =
				Utils::toGLMMatrix(rawMesh.mBones[boneIndex]->mOffsetMatrix);

			bonesByName[boneName] = bone;
			boneId = bone.Id;
		} else {
			boneId = bonesByName[boneName].Id;
		}

		auto weights = rawMesh.mBones[boneIndex]->mWeights;
		int numWeights = rawMesh.mBones[boneIndex]->mNumWeights;

		for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex) {
			int vertexId = weights[weightIndex].mVertexId;
			float weight = weights[weightIndex].mWeight;

			Vertex& vertex = vertices[vertexId];
			addVertexBone(vertex, boneId, weight);
		}
	}
}

static std::optional<Texture> readTexture(const aiMesh& rawMesh,
	const aiScene& scene, std::string& directory,
	std::map<int, Texture>& texturesByMaterialIndex) {
	int materialIndex = rawMesh.mMaterialIndex;

	if (materialIndex < 0)
		return {};

	auto hasTexture = texturesByMaterialIndex.count(materialIndex) > 0;

	if (hasTexture)
		return texturesByMaterialIndex.find(materialIndex)->second;

	aiString texturePath;

	scene.mMaterials[materialIndex]->GetTexture(
		aiTextureType_DIFFUSE, 0, &texturePath);

	auto embeddedTextureRequest = scene.GetEmbeddedTexture(texturePath.C_Str());

	std::optional<Texture> finalTexture;

	if (embeddedTextureRequest) {
		finalTexture = Texture(
			embeddedTextureRequest->pcData, embeddedTextureRequest->mWidth);
	} else {
		finalTexture =
			Texture(directory + "/" + std::string(texturePath.C_Str()));
	}

	texturesByMaterialIndex.insert_or_assign(materialIndex, *finalTexture);

	return finalTexture;
}

static void buildBoneHierarchy(Bone** rootBone, Bone* parentBone,
	const aiNode& rawNode, std::map<std::string, Bone>& bonesByName) {
	std::string nodeName = rawNode.mName.data;
	auto boneResult = bonesByName.find(nodeName);
	Bone* bone = &boneResult->second;
	Bone* newParentBone = parentBone;

	if (boneResult != bonesByName.end()) {
		bone->DefaultTransform = toGLMMatrix(rawNode.mTransformation);

		if (*rootBone == nullptr) {
			*rootBone = bone;
		} else {
			parentBone->Children.push_back(bone);
		}

		newParentBone = bone;
	}

	for (int childIndex = 0; childIndex < rawNode.mNumChildren; childIndex++) {
		buildBoneHierarchy(rootBone, newParentBone,
			*rawNode.mChildren[childIndex], bonesByName);
	}
}

static void readAnimations(const aiScene& rawScene,
	std::map<std::string, Bone>& bonesByName, Bone* skeleton,
	std::vector<Animation>& animations) {
	for (int animationIndex = 0; animationIndex < rawScene.mNumAnimations;
		 animationIndex++) {

		Animation animation;
		animation.Duration = rawScene.mAnimations[animationIndex]->mDuration;
		animation.TicksPerSecond =
			rawScene.mAnimations[animationIndex]->mTicksPerSecond;

		int boneAnimationCount =
			rawScene.mAnimations[animationIndex]->mNumChannels;

		for (int boneAnimationIndex = 0;
			 boneAnimationIndex < boneAnimationCount; boneAnimationIndex++) {

			auto channel = rawScene.mAnimations[animationIndex]
							   ->mChannels[boneAnimationIndex];

			std::string boneName = channel->mNodeName.data;

			if (bonesByName.find(boneName) == bonesByName.end()) {
				bonesByName[boneName].Id = bonesByName.size();
			}

			BoneAnimation boneAnimation;
			boneAnimation.BoneName = channel->mNodeName.data;
			boneAnimation.BoneId = bonesByName[boneAnimation.BoneName].Id;

			for (int positionIndex = 0;
				 positionIndex < channel->mNumPositionKeys; ++positionIndex) {

				aiVector3D rawPosition =
					channel->mPositionKeys[positionIndex].mValue;

				float timestamp = channel->mPositionKeys[positionIndex].mTime;

				KeyFramePosition data;
				data.Position = Utils::toGLMVec3(rawPosition);
				data.Timestamp = timestamp;
				boneAnimation.Positions.push_back(data);
			}

			for (int rotationIndex = 0;
				 rotationIndex < channel->mNumRotationKeys; ++rotationIndex) {

				aiQuaternion rawRotation =
					channel->mRotationKeys[rotationIndex].mValue;

				float timestamp = channel->mRotationKeys[rotationIndex].mTime;
				KeyFrameRotation data;
				data.Orientation = Utils::toGLMQuat(rawRotation);
				data.Timestamp = timestamp;
				boneAnimation.Rotations.push_back(data);
			}

			for (int keyIndex = 0; keyIndex < channel->mNumScalingKeys;
				 ++keyIndex) {

				aiVector3D rawScale = channel->mScalingKeys[keyIndex].mValue;
				float timestamp = channel->mScalingKeys[keyIndex].mTime;

				KeyFrameScale data;
				data.Scale = Utils::toGLMVec3(rawScale);
				data.Timestamp = timestamp;
				boneAnimation.Scales.push_back(data);
			}

			animation.BoneAnimations.push_back(boneAnimation);
		}

		animation.Skeleton = skeleton;
		animations.push_back(animation);
	}
}

} // namespace Utils

Model::Model(const std::string& file, bool flip) {
	Assimp::Importer importer;
	unsigned int flags = aiProcess_Triangulate | aiProcess_LimitBoneWeights;

	if (flip) {
		flags |= aiProcess_FlipUVs;
	}

	const aiScene* scene = importer.ReadFile(file, flags);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
		!scene->mRootNode) {
		MATTENGINE_ASSERT(false, "Failed to load model: %s - %s", file.c_str(),
			importer.GetErrorString());
	}

	std::string directory = file.substr(0, file.find_last_of('/'));
	unsigned int boneCounter = 0;
	std::map<int, Texture> texturesByMaterialIndex;

	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

		Utils::readVertices(*mesh, vertices);
		Utils::readIndices(*mesh, indices);

		std::optional<Texture> texture = Utils::readTexture(
			*mesh, *scene, directory, texturesByMaterialIndex);

		Utils::readBones(*mesh, BonesByName, vertices);

		Meshes.push_back(Mesh(vertices, indices, texture));
	}

	Utils::buildBoneHierarchy(
		&Skeleton, nullptr, *scene->mRootNode, BonesByName);

	Utils::readAnimations(*scene, BonesByName, Skeleton, Animations);
}
