#include "ImGuiComponentEditor.h"

#include "AnimationComponent.h"
#include "ColourComponent.h"
#include "TextureComponent.h"
#include "TransformComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace MattEngine::ImGuiCustom;

static void BoneNode(MattEngine::Bone& bone) {
	if (ImGui::TreeNode(bone.Name.c_str())) {

		for (auto& childBone : bone.Children) {
			BoneNode(*childBone);
		}

		ImGui::TreePop();
	}
}

void ComponentEditor::render(Entity& entity, bool newEntity) {
	if (!entity)
		return;

	ImGui::Begin("Components");

	if (entity.hasComponent<TransformComponent>()) {
		TransformComponent& transform =
			entity.getComponent<TransformComponent>();

		if (ImGui::CollapsingHeader(
				"Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::DragFloat3("Position", glm::value_ptr(transform.Position),
				0.1f, -1000.0f, 1000.0f);
			ImGui::DragFloat3("Size", glm::value_ptr(transform.Size), 0.1f,
				-1000.0f, 1000.0f);

			m_quatEditor.render(transform.Rotation, newEntity);
		}
	}

	if (entity.hasComponent<ColourComponent>()) {
		if (ImGui::CollapsingHeader("Colour", ImGuiTreeNodeFlags_DefaultOpen)) {
			ImGui::ColorEdit3("Colour",
				glm::value_ptr(entity.getComponent<ColourComponent>().Colour));
		}
	}

	if (entity.hasComponent<TextureComponent>()) {
		TextureComponent& texture = entity.getComponent<TextureComponent>();

		if (ImGui::CollapsingHeader(
				"Texture", ImGuiTreeNodeFlags_DefaultOpen)) {

			char texturePath[300];
			strcpy(texturePath, texture.Path.c_str());

			if (ImGui::InputText("Path", texturePath, 300)) {
				texture.Path = texturePath;
			}

			if (ImGui::BeginDragDropTarget()) {

				if (const ImGuiPayload* payload =
						ImGui::AcceptDragDropPayload("FILE_PATH")) {
					texture.Path = (char*)payload->Data;
				}

				ImGui::EndDragDropTarget();
			}
		}
	}

	if (entity.hasComponent<AnimationComponent>()) {
		if (ImGui::CollapsingHeader(
				"Animation", ImGuiTreeNodeFlags_DefaultOpen)) {

			AnimationComponent& animation =
				entity.getComponent<AnimationComponent>();

			float currentTime = animation.Animator.getCurrentTime();
			float duration = animation.Animation->Duration;
			float ticksPerSecond = animation.Animation->TicksPerSecond;

			ImGui::DragFloat("Duration", &duration);
			ImGui::DragFloat("Ticks Per Second", &ticksPerSecond);
			ImGui::DragFloat("Current Time", &currentTime);

			if (ImGui::TreeNode("Skeleton")) {
				BoneNode(*animation.Animation->Skeleton);
				ImGui::TreePop();
			}
		}
	}

	ImGui::End();
}