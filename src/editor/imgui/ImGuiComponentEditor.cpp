#include "ImGuiComponentEditor.h"

#include "AnimationComponent.h"
#include "ColourComponent.h"
#include "PointLightComponent.h"
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

static void transformEditor(
	Entity& entity, QuatEditor& quatEditor, bool newEntity) {
	if (!entity.hasComponent<TransformComponent>())
		return;

	TransformComponent& transform = entity.getComponent<TransformComponent>();

	if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::DragFloat3("Position", glm::value_ptr(transform.Position), 0.1f,
			-1000.0f, 1000.0f);
		ImGui::DragFloat3(
			"Size", glm::value_ptr(transform.Size), 0.1f, -1000.0f, 1000.0f);

		quatEditor.render(transform.Rotation, newEntity);
	}
}

static void colourEditor(Entity& entity) {
	if (!entity.hasComponent<ColourComponent>())
		return;

	ColourComponent& colour = entity.getComponent<ColourComponent>();

	if (ImGui::CollapsingHeader("Colour", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::ColorEdit3("Colour", glm::value_ptr(colour.Colour));
		ImGui::Checkbox("Emissive", &colour.Emissive);

		if (colour.Emissive) {
			ImGui::DragFloat("Intensity", &colour.Intensity);
		}
	}
}

static void textureEditor(Entity& entity) {
	if (!entity.hasComponent<TextureComponent>())
		return;

	TextureComponent& texture = entity.getComponent<TextureComponent>();

	if (ImGui::CollapsingHeader("Texture", ImGuiTreeNodeFlags_DefaultOpen)) {

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

		int tileType = texture.UseTileSize ? 1 : 0;
		ImGui::RadioButton("Tile Count##TileCountOption", &tileType, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Tile Size##TileSizeOption", &tileType, 1);
		texture.UseTileSize = tileType == 1;

		if (texture.UseTileSize) {
			ImGui::DragFloat("Tile Size", &texture.TileSize);
		} else {
			ImGui::DragInt("Tile Count", (int*)(&texture.TileCount));
		}
	}
}

static void animationEditor(Entity& entity) {
	if (!entity.hasComponent<AnimationComponent>())
		return;

	if (ImGui::CollapsingHeader("Animation", ImGuiTreeNodeFlags_DefaultOpen)) {

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

static void pointLightEditor(Entity& entity) {
	if (!entity.hasComponent<PointLightComponent>())
		return;

	if (ImGui::CollapsingHeader(
			"Point Light", ImGuiTreeNodeFlags_DefaultOpen)) {

		PointLightComponent& pointLight =
			entity.getComponent<PointLightComponent>();

		ImGui::DragFloat3("Position", glm::value_ptr(pointLight.Light.Position),
			0.1f, -1000.0f, 1000.0f);

		ImGui::ColorEdit3("Ambient", glm::value_ptr(pointLight.Light.Ambient));
		ImGui::ColorEdit3("Diffuse", glm::value_ptr(pointLight.Light.Diffuse));
		ImGui::ColorEdit3(
			"Specular", glm::value_ptr(pointLight.Light.Specular));

		ImGui::DragFloat(
			"Constant", &pointLight.Light.Constant, 0.005f, 0.0f, 1.0f);
		ImGui::DragFloat(
			"Linear", &pointLight.Light.Linear, 0.005f, 0.0f, 1.0f);
		ImGui::DragFloat(
			"Quadratic", &pointLight.Light.Quadratic, 0.005f, 0.0f, 1.0f);
	}
}

template <typename T>
static void addComponentButton(Entity& entity, const std::string& name) {
	if (entity.hasComponent<T>())
		return;

	if (ImGui::Selectable(name.c_str())) {
		entity.addComponent<T>();
	}
}

void ComponentEditor::render(Entity& entity, bool newEntity) {
	if (!entity)
		return;

	ImGui::Begin("Components");

	if (ImGui::BeginPopupContextItem("AddComponent")) {
		addComponentButton<ColourComponent>(entity, "Colour");
		addComponentButton<TextureComponent>(entity, "Texture");
		addComponentButton<TransformComponent>(entity, "Transform");
		addComponentButton<PointLightComponent>(entity, "Point Light");
		ImGui::EndPopup();
	}

	if (ImGui::Button("Add Component")) {
		ImGui::OpenPopup("AddComponent");
	}

	transformEditor(entity, m_quatEditor, newEntity);
	colourEditor(entity);
	textureEditor(entity);
	pointLightEditor(entity);
	animationEditor(entity);

	ImGui::End();
}