#include "PerspectiveCamera.h"

#include "ModelComponent.h"
#include "ModelStore.h"
#include "Scene.h"
#include "TransformComponent.h"
#include "Window.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

using namespace MattEngine;

PerspectiveCamera::PerspectiveCamera() {
	invalidate();
	m_controller.init(m_position, m_rotation);
}

PerspectiveCamera::PerspectiveCamera(float nearPlane, float farPlane)
	: m_nearPlane(nearPlane), m_farPlane(farPlane) {
	invalidate();
	m_controller.init(m_position, m_rotation);
}

void PerspectiveCamera::onUpdate(float deltaTime) {
	if (!m_useController)
		return;

	m_controller.onUpdate(deltaTime);
	invalidate();
}

void PerspectiveCamera::invalidate() {
	glm::vec3 direction = glm::rotate(m_rotation, glm::vec3(0.0f, 0.0f, 1.0f));

	m_forward = glm::normalize(direction);
	m_view = glm::lookAt(m_position, m_position + m_forward, m_up);
	m_projection = glm::perspective(
		glm::radians(m_angle), m_aspectRatio, m_nearPlane, m_farPlane);
}

OrthoCamera PerspectiveCamera::getBoundingOrtho(
	const glm::vec3& viewDirection, Scene& scene) {

	glm::mat4 lightSpaceMatrix = glm::lookAt(
		glm::normalize(viewDirection), {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f});

	const std::vector<glm::vec3>& cameraBounds = getBounds();

	float left = INFINITY;
	float right = -INFINITY;
	float top = -INFINITY;
	float bottom = INFINITY;
	float back = INFINITY;
	float front = -INFINITY;

	for (auto& bound : cameraBounds) {
		glm::vec3 lightBound = lightSpaceMatrix * glm::vec4(bound, 1.0);

		if (lightBound.x < left) {
			left = lightBound.x;
		}

		if (lightBound.x > right) {
			right = lightBound.x;
		}

		if (lightBound.y < bottom) {
			bottom = lightBound.y;
		}

		if (lightBound.y > top) {
			top = lightBound.y;
		}

		if (lightBound.z < back) {
			back = lightBound.z;
		}
	}

	for (auto& entity : scene.getAllEntities()) {
		if (!entity.hasComponent<TransformComponent>())
			continue;

		if (entity.getComponent<TagComponent>().Tag == "Floor")
			continue;

		TransformComponent& transform =
			entity.getComponent<TransformComponent>();

		glm::vec3 lightPosition =
			lightSpaceMatrix * glm::vec4(transform.Position, 1.0);

		float offset = 0;

		if (entity.hasComponent<ModelComponent>()) {
			ModelComponent& modelComponent =
				entity.getComponent<ModelComponent>();
			Model model =
				ModelStore::getInstance().getModel(modelComponent.Path);

			float width = model.Bounds[MODEL_BOUND_RIGHT] -
						  model.Bounds[MODEL_BOUND_LEFT];
			float height = model.Bounds[MODEL_BOUND_TOP] -
						   model.Bounds[MODEL_BOUND_BOTTOM];
			float depth = model.Bounds[MODEL_BOUND_BACK] -
						  model.Bounds[MODEL_BOUND_FRONT];

			offset = ((width * transform.Size.x) + (height * transform.Size.y) +
						 (depth * transform.Size.z)) /
					 2;
		} else {
			offset =
				(transform.Size.x + transform.Size.y + transform.Size.z) / 2;
		}

		lightPosition.z += offset;

		if (lightPosition.z > front) {
			front = lightPosition.z;
		}
	}

	float width = right - left;
	float halfWidth = width / 2.0f;
	float height = top - bottom;
	float halfHeight = height / 2.0f;
	float depth = front - back;

	glm::vec3 frontTotal =
		glm::vec3(left, bottom, front) + glm::vec3(left, top, front) +
		glm::vec3(right, top, front) + glm::vec3(right, bottom, front);

	glm::vec3 total = frontTotal + glm::vec3(left, bottom, back) +
					  glm::vec3(left, top, back) + glm::vec3(right, top, back) +
					  glm::vec3(right, bottom, back);

	glm::vec3 lightFrontCenter = frontTotal / 4.0f;
	glm::vec3 lightCenter = total / 8.0f;

	glm::vec3 frontCenter =
		glm::inverse(lightSpaceMatrix) * glm::vec4(lightFrontCenter, 1.0);
	glm::vec3 center =
		glm::inverse(lightSpaceMatrix) * glm::vec4(lightCenter, 1.0);

	OrthoCamera ortho =
		OrthoCamera({-halfWidth, halfWidth, -halfHeight, halfHeight},
			{0.0f, depth}, frontCenter, center);

	return ortho;
}