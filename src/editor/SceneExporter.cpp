#include "SceneExporter.h"

#include "ColourComponent.h"
#include "Game.h"
#include "ModelComponent.h"
#include "PointLightComponent.h"
#include "TextureComponent.h"
#include "TransformComponent.h"

#include "yaml-cpp/yaml.h"

#include <fstream>
#include <iostream>

using namespace MattEngine;

static void serialiseVec3(const glm::vec3& vec, YAML::Emitter& out) {
	out << YAML::Flow << YAML::BeginSeq << vec.x << vec.y << vec.z
		<< YAML::EndSeq;
}

static void serialiseQuat(const glm::quat& quat, YAML::Emitter& out) {
	out << YAML::Flow << YAML::BeginSeq << quat.x << quat.y << quat.z << quat.w
		<< YAML::EndSeq;
}

static void serializeTagComponent(
	const TagComponent& tagComponent, YAML::Emitter& out) {
	out << YAML::Key << "Tag";
	out << YAML::Value << tagComponent.Tag;
}

static void serializeTransformComponent(
	const TransformComponent& transformComponent, YAML::Emitter& out) {
	out << YAML::Key << "Transform";
	out << YAML::Value;
	out << YAML::BeginMap;
	out << YAML::Key << "Position";
	out << YAML::Value;
	serialiseVec3(transformComponent.Position, out);
	out << YAML::Key << "Size";
	out << YAML::Value;
	serialiseVec3(transformComponent.Size, out);
	out << YAML::Key << "Rotation";
	out << YAML::Value;
	serialiseQuat(transformComponent.Rotation, out);
	out << YAML::EndMap;
}

static void serializeColourComponent(
	const ColourComponent& colourComponent, YAML::Emitter& out) {
	out << YAML::Key << "Colour";
	out << YAML::Value;
	serialiseVec3(colourComponent.Colour, out);
}

static void serializeTextureComponent(
	const TextureComponent& textureComponent, YAML::Emitter& out) {
	out << YAML::Key << "Texture";
	out << YAML::Value;
	out << YAML::BeginMap;
	out << YAML::Key << "Path";
	out << YAML::Value << textureComponent.Path;
	out << YAML::Key << "Tile Count";
	out << YAML::Value << textureComponent.TileCount;
	out << YAML::EndMap;
}

static void serializeModelComponent(
	const ModelComponent& modelComponent, YAML::Emitter& out) {
	out << YAML::Key << "Model";
	out << YAML::Value;
	out << YAML::BeginMap;
	out << YAML::Key << "Path";
	out << YAML::Value << modelComponent.Path;
	out << YAML::EndMap;
}

static void serializePointLightComponent(
	const PointLightComponent& pointLightComponent, YAML::Emitter& out) {
	out << YAML::Key << "Point Light";
	out << YAML::Value;
	out << YAML::BeginMap;
	out << YAML::Key << "Position";
	out << YAML::Value;
	serialiseVec3(pointLightComponent.Light.Position, out);
	out << YAML::Key << "Ambient";
	out << YAML::Value;
	serialiseVec3(pointLightComponent.Light.Ambient, out);
	out << YAML::Key << "Diffuse";
	out << YAML::Value;
	serialiseVec3(pointLightComponent.Light.Diffuse, out);
	out << YAML::Key << "Specular";
	out << YAML::Value;
	serialiseVec3(pointLightComponent.Light.Specular, out);
	out << YAML::Key << "Constant";
	out << YAML::Value << pointLightComponent.Light.Constant;
	out << YAML::Key << "Linear";
	out << YAML::Value << pointLightComponent.Light.Linear;
	out << YAML::Key << "Quadratic";
	out << YAML::Value << pointLightComponent.Light.Quadratic;
	out << YAML::EndMap;
}

static void serialiseEntity(Entity& entity, YAML::Emitter& out) {
	out << YAML::BeginMap;

	if (entity.hasComponent<TagComponent>()) {
		TagComponent& tag = entity.getComponent<TagComponent>();
		serializeTagComponent(tag, out);
	}

	if (entity.hasComponent<TransformComponent>()) {
		TransformComponent& transform =
			entity.getComponent<TransformComponent>();
		serializeTransformComponent(transform, out);
	}

	if (entity.hasComponent<ColourComponent>()) {
		ColourComponent& colour = entity.getComponent<ColourComponent>();
		serializeColourComponent(colour, out);
	}

	if (entity.hasComponent<TextureComponent>()) {
		TextureComponent& texture = entity.getComponent<TextureComponent>();
		serializeTextureComponent(texture, out);
	}

	if (entity.hasComponent<ModelComponent>()) {
		ModelComponent& model = entity.getComponent<ModelComponent>();
		serializeModelComponent(model, out);
	}

	if (entity.hasComponent<PointLightComponent>()) {
		PointLightComponent& pointLight =
			entity.getComponent<PointLightComponent>();
		serializePointLightComponent(pointLight, out);
	}

	out << YAML::EndMap;
}

void SceneExporter::save(Scene& scene, const std::string& path) {
	YAML::Emitter out;

	out << YAML::BeginSeq;

	for (auto& entity : scene.getAllEntities()) {
		serialiseEntity(entity, out);
	}

	out << YAML::EndSeq;

	std::ofstream file;
	file.open(path);
	file.write(out.c_str(), out.size());
	file.close();
}