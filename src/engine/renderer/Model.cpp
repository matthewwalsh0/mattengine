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

static void buildAnimationNode(AnimationNode& node, const aiNode& rawNode) {
	node.BoneName = rawNode.mName.data;
	node.Transform = toGLMMatrix(rawNode.mTransformation);
	node.ChildrenCount = rawNode.mNumChildren;

	for (int childIndex = 0; childIndex < rawNode.mNumChildren; childIndex++) {
		AnimationNode childNode;
		buildAnimationNode(childNode, *rawNode.mChildren[childIndex]);
		node.Children.push_back(childNode);
	}
}

} // namespace Utils

Model::Model(const std::string& file, bool flip) {
	Assimp::Importer importer;
	unsigned int flags = aiProcess_Triangulate;

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
	std::map<int, Texture*> texturesByMaterialIndex;

	for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[i];
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		Texture* texture = nullptr;

		for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
			Vertex vertex;
			vertex.Position = Utils::toGLMVec3(mesh->mVertices[j]);
			vertex.Normal = Utils::toGLMVec3(mesh->mNormals[j]);

			for (int t = 0; t < 4; t++) {
				vertex.BoneIds[t] = -1;
				vertex.BoneWeights[t] = 0.0f;
			}

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

		int materialIndex = mesh->mMaterialIndex;
		auto hasTexture = texturesByMaterialIndex.count(materialIndex);

		if (materialIndex >= 0) {

			if (hasTexture == 0) {
				aiString texturePath;

				scene->mMaterials[mesh->mMaterialIndex]->GetTexture(
					aiTextureType_DIFFUSE, 0, &texturePath);

				if (auto textureRequest =
						scene->GetEmbeddedTexture(texturePath.C_Str())) {

					texture = new Texture(
						textureRequest->pcData, textureRequest->mWidth);
				} else {
					texture = new Texture(
						directory + "/" + std::string(texturePath.C_Str()));
				}

				texturesByMaterialIndex[materialIndex] = texture;
			} else {
				texture = texturesByMaterialIndex[materialIndex];
			}
		}

		for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex) {
			int boneId = -1;
			std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();

			if (BonesByName.find(boneName) == BonesByName.end()) {
				Bone bone;
				bone.Id = boneCounter++;
				bone.ModelToLocalTransform =
					Utils::toGLMMatrix(mesh->mBones[boneIndex]->mOffsetMatrix);

				BonesByName[boneName] = bone;
				boneId = bone.Id;
			} else {
				boneId = BonesByName[boneName].Id;
			}

			auto weights = mesh->mBones[boneIndex]->mWeights;
			int numWeights = mesh->mBones[boneIndex]->mNumWeights;

			for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex) {
				int vertexId = weights[weightIndex].mVertexId;
				float weight = weights[weightIndex].mWeight;

				Vertex& vertex = vertices[vertexId];

				for (int k = 0; k < 4; ++k) {
					if (vertex.BoneIds[k] < 0 && weight != 0.0f) {
						vertex.BoneWeights[k] = weight;
						vertex.BoneIds[k] = boneId;
						break;
					}
				}
			}
		}

		Meshes.push_back(Mesh(vertices, indices, texture));
	}

	for (int animationIndex = 0; animationIndex < scene->mNumAnimations;
		 animationIndex++) {

		Animation animation;
		animation.Duration = scene->mAnimations[animationIndex]->mDuration;
		animation.TicksPerSecond =
			scene->mAnimations[animationIndex]->mTicksPerSecond;

		int boneAnimationCount =
			scene->mAnimations[animationIndex]->mNumChannels;

		for (int boneAnimationIndex = 0;
			 boneAnimationIndex < boneAnimationCount; boneAnimationIndex++) {

			auto channel = scene->mAnimations[animationIndex]
							   ->mChannels[boneAnimationIndex];

			std::string boneName = channel->mNodeName.data;

			if (BonesByName.find(boneName) == BonesByName.end()) {
				BonesByName[boneName].Id = BonesByName.size();
			}

			BoneAnimation boneAnimation;
			boneAnimation.BoneName = channel->mNodeName.data;
			boneAnimation.BoneId = BonesByName[boneAnimation.BoneName].Id;

			boneAnimation.PositionCount = channel->mNumPositionKeys;

			for (int positionIndex = 0;
				 positionIndex < boneAnimation.PositionCount; ++positionIndex) {

				aiVector3D rawPosition =
					channel->mPositionKeys[positionIndex].mValue;

				float timestamp = channel->mPositionKeys[positionIndex].mTime;

				KeyFramePosition data;
				data.Position = Utils::toGLMVec3(rawPosition);
				data.Timestamp = timestamp;
				boneAnimation.Positions.push_back(data);
			}

			boneAnimation.RotationCount = channel->mNumRotationKeys;

			for (int rotationIndex = 0;
				 rotationIndex < boneAnimation.RotationCount; ++rotationIndex) {

				aiQuaternion rawRotation =
					channel->mRotationKeys[rotationIndex].mValue;

				float timestamp = channel->mRotationKeys[rotationIndex].mTime;
				KeyFrameRotation data;
				data.Orientation = Utils::toGLMQuat(rawRotation);
				data.Timestamp = timestamp;
				boneAnimation.Rotations.push_back(data);
			}

			boneAnimation.ScaleCount = channel->mNumScalingKeys;

			for (int keyIndex = 0; keyIndex < boneAnimation.ScaleCount;
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

		Utils::buildAnimationNode(animation.RootNode, *(scene->mRootNode));

		animation.BonesByName = BonesByName;

		Animations.push_back(animation);
	}
}
