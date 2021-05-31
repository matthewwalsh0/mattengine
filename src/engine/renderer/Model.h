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
	glm::mat4 ModelToLocalTransform;
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
	int PositionCount;
	int RotationCount;
	int ScaleCount;
	glm::mat4 LocalTransform = glm::mat4(1.0f);
	std::string BoneName;
	int BoneId;
};

struct AnimationNode {
	glm::mat4 Transform;
	std::string BoneName;
	int ChildrenCount;
	std::vector<AnimationNode> Children;
};

struct Animation {
	float Duration;
	int TicksPerSecond;
	std::vector<BoneAnimation> BoneAnimations;
	AnimationNode RootNode;
	std::map<std::string, Bone> BonesByName;
};

class Model {
public:
	std::vector<Mesh> Meshes;
	std::map<std::string, Bone> BonesByName;
	std::vector<Animation> Animations;

public:
	Model(const std::string& file, bool flip = false);
};
} // namespace MattEngine

#endif