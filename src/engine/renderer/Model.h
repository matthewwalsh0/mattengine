#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

#include <map>
#include <string>
#include <vector>

#include <glm/gtx/quaternion.hpp>

namespace MattEngine {

const unsigned int MODEL_BOUND_LEFT = 0;
const unsigned int MODEL_BOUND_RIGHT = 1;
const unsigned int MODEL_BOUND_TOP = 2;
const unsigned int MODEL_BOUND_BOTTOM = 3;
const unsigned int MODEL_BOUND_FRONT = 4;
const unsigned int MODEL_BOUND_BACK = 5;

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
	Model(const std::string& file, bool flip = false);

	const glm::vec3* getBounds() {
		if (!m_boundPositionsSet) {
			float left = Bounds[MODEL_BOUND_LEFT];
			float right = Bounds[MODEL_BOUND_RIGHT];
			float top = Bounds[MODEL_BOUND_TOP];
			float bottom = Bounds[MODEL_BOUND_BOTTOM];
			float front = Bounds[MODEL_BOUND_FRONT];
			float back = Bounds[MODEL_BOUND_BACK];

			m_boundPositions[0] = {left, bottom, front};
			m_boundPositions[1] = {left, top, front};
			m_boundPositions[2] = {right, top, front};
			m_boundPositions[3] = {right, bottom, front};
			m_boundPositions[4] = {left, bottom, back};
			m_boundPositions[5] = {left, top, back};
			m_boundPositions[6] = {right, top, back};
			m_boundPositions[7] = {right, bottom, back};

			m_boundPositionsSet = true;
		}

		return m_boundPositions;
	}

	const glm::vec3 getCenterOffset() {
		if (!m_centerPositionSet) {
			const glm::vec3* bounds = getBounds();

			glm::vec3 total = {0.0f, 0.0f, 0.0f};

			for (int i = 0; i < 8; i++) {
				total += bounds[i];
			}

			m_centerPosition = total / 8.0f;
			m_centerPositionSet = true;
		}

		return m_centerPosition;
	}

public:
	std::vector<Mesh> Meshes;
	std::vector<Animation> Animations;
	std::map<std::string, Bone> BonesByName;
	Bone* Skeleton = nullptr;
	float Bounds[6];

private:
	glm::vec3 m_boundPositions[8];
	glm::vec3 m_centerPosition;
	bool m_boundPositionsSet = false;
	bool m_centerPositionSet = false;
};

} // namespace MattEngine

#endif