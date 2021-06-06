#include "Animator.h"

#include "Log.h"

using namespace MattEngine;

namespace Utils {

static BoneAnimation* getBoneAnimation(
	Animation& animation, const std::string& name) {

	auto iter = std::find_if(animation.BoneAnimations.begin(),
		animation.BoneAnimations.end(),
		[&](const BoneAnimation& boneAnimation) {
			return boneAnimation.BoneName == name;
		});

	if (iter == animation.BoneAnimations.end())
		return nullptr;
	else
		return &(*iter);
}

static int getPositionIndex(
	float animationTime, std::vector<KeyFramePosition>& positions) {

	for (int index = 0; index < positions.size() - 1; ++index) {
		if (animationTime < positions[index + 1].Timestamp)
			return index;
	}

	return positions.size() - 1;
}

static int getRotationIndex(
	float animationTime, std::vector<KeyFrameRotation>& rotations) {

	for (int index = 0; index < rotations.size() - 1; ++index) {
		if (animationTime < rotations[index + 1].Timestamp)
			return index;
	}

	return rotations.size() - 1;
}

static int getScaleIndex(
	float animationTime, std::vector<KeyFrameScale>& scales) {

	for (int index = 0; index < scales.size() - 1; ++index) {
		if (animationTime < scales[index + 1].Timestamp)
			return index;
	}

	return scales.size() - 1;
}

static float getScaleFactor(
	float lastTimestamp, float nextTimestamp, float animationTime) {

	float scaleFactor = 0.0f;
	float midWayLength = animationTime - lastTimestamp;
	float framesDiff = nextTimestamp - lastTimestamp;

	scaleFactor = midWayLength / framesDiff;

	return scaleFactor;
}

static glm::mat4 interpolatePosition(
	float animationTime, BoneAnimation& boneAnimation) {
	int positionSize = boneAnimation.Positions.size();

	if (1 == positionSize)
		return glm::translate(
			glm::mat4(1.0f), boneAnimation.Positions[0].Position);

	int p0Index = getPositionIndex(animationTime, boneAnimation.Positions);
	int p1Index = p0Index == positionSize - 1 ? 0 : p0Index + 1;

	float scaleFactor =
		getScaleFactor(boneAnimation.Positions[p0Index].Timestamp,
			boneAnimation.Positions[p1Index].Timestamp, animationTime);

	glm::vec3 finalPosition =
		glm::mix(boneAnimation.Positions[p0Index].Position,
			boneAnimation.Positions[p1Index].Position, scaleFactor);

	return glm::translate(glm::mat4(1.0f), finalPosition);
}

static glm::mat4 interpolateRotation(
	float animationTime, BoneAnimation& boneAnimation) {
	int rotationCount = boneAnimation.Rotations.size();

	if (1 == rotationCount) {
		auto rotation = glm::normalize(boneAnimation.Rotations[0].Orientation);
		return glm::toMat4(rotation);
	}

	int p0Index = getRotationIndex(animationTime, boneAnimation.Rotations);
	int p1Index = p0Index == rotationCount - 1 ? 0 : p0Index + 1;

	float scaleFactor =
		getScaleFactor(boneAnimation.Rotations[p0Index].Timestamp,
			boneAnimation.Rotations[p1Index].Timestamp, animationTime);

	glm::quat finalRotation =
		glm::slerp(boneAnimation.Rotations[p0Index].Orientation,
			boneAnimation.Rotations[p1Index].Orientation, scaleFactor);

	return glm::toMat4(glm::normalize(finalRotation));
}

static glm::mat4 interpolateScale(
	float animationTime, BoneAnimation& boneAnimation) {
	int scaleCount = boneAnimation.Scales.size();

	if (1 == scaleCount)
		return glm::scale(glm::mat4(1.0f), boneAnimation.Scales[0].Scale);

	int p0Index = getScaleIndex(animationTime, boneAnimation.Scales);
	int p1Index = p0Index == scaleCount - 1 ? 0 : p0Index + 1;

	float scaleFactor = getScaleFactor(boneAnimation.Scales[p0Index].Timestamp,
		boneAnimation.Scales[p1Index].Timestamp, animationTime);

	glm::vec3 finalScale = glm::mix(boneAnimation.Scales[p0Index].Scale,
		boneAnimation.Scales[p1Index].Scale, scaleFactor);

	return glm::scale(glm::mat4(1.0f), finalScale);
}

static void updateBoneTransform(float animationTime, Animation& animation,
	const Bone& bone, glm::mat4 parentTransform,
	std::vector<glm::mat4>& boneTransforms) {

	glm::mat4 boneTransform = bone.DefaultTransform;
	BoneAnimation* boneAnimation =
		Utils::getBoneAnimation(animation, bone.Name);

	if (boneAnimation) {
		glm::mat4 translation =
			Utils::interpolatePosition(animationTime, *boneAnimation);

		glm::mat4 rotation =
			Utils::interpolateRotation(animationTime, *boneAnimation);

		glm::mat4 scale =
			Utils::interpolateScale(animationTime, *boneAnimation);

		boneTransform = translation * rotation * scale;
	}

	glm::mat4 globalTransformation = parentTransform * boneTransform;
	boneTransforms[bone.Id] = globalTransformation * bone.ModelToLocalTransform;

	for (int childIndex = 0; childIndex < bone.Children.size(); childIndex++)
		updateBoneTransform(animationTime, animation,
			*(bone.Children[childIndex]), globalTransformation, boneTransforms);
}

} // namespace Utils

Animator::Animator() {
	m_currentTime = 0.0f;
	m_boneTransforms.reserve(100);

	for (int i = 0; i < 100; i++)
		m_boneTransforms.push_back(glm::mat4(1.0f));
}

void Animator::onUpdate(float deltaTime) {
	if (!m_animation)
		return;

	m_currentTime += 1000 * deltaTime;
	m_currentTime = fmod(m_currentTime, m_animation->Duration);

	Utils::updateBoneTransform(m_currentTime, *m_animation,
		*m_animation->Skeleton, glm::mat4(1.0f), m_boneTransforms);
}

void Animator::playAnimation(Animation& animation) {
	m_animation = &animation;
	m_currentTime = 0.0f;
}

std::vector<glm::mat4>& Animator::getBoneTransforms() {
	return m_boneTransforms;
}
