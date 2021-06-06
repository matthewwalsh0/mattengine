#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

#include <map>
#include <string>
#include <vector>

#include <glm/gtx/quaternion.hpp>

namespace MattEngine {

struct Bone {
	int Id;
	std::string Name;
	glm::mat4 ModelToLocalTransform;
	glm::mat4 DefaultTransform;
	std::vector<Bone*> Children;
};

struct KeyFramePosition {
	glm::vec3 Position;
	float Timestamp;
};

struct KeyFrameRotation {
	glm::quat Orientation;
	float Timestamp;
};

struct KeyFrameScale {
	glm::vec3 Scale;
	float Timestamp;
};

struct BoneAnimation {
	std::vector<KeyFramePosition> Positions;
	std::vector<KeyFrameRotation> Rotations;
	std::vector<KeyFrameScale> Scales;
	glm::mat4 LocalTransform = glm::mat4(1.0f);
	std::string BoneName;
	int BoneId;
};

struct Animation {
	float Duration;
	int TicksPerSecond;
	std::vector<BoneAnimation> BoneAnimations;
	Bone* Skeleton = nullptr;
};

class Model {
public:
	std::vector<Mesh> Meshes;
	std::vector<Animation> Animations;
	std::map<std::string, Bone> BonesByName;
	Bone* Skeleton = nullptr;

public:
	Model(const std::string& file, bool flip = false);
};
} // namespace MattEngine

#endif