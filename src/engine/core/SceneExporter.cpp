#include "SceneExporter.h"

#include "ColourComponent.h"
#include "Game.h"
#include "ModelComponent.h"
#include "TextureComponent.h"
#include "TransformComponent.h"

#include "yaml-cpp/yaml.h"

#include <fstream>
#include <iostream>

using namespace MattEngine;

static void serialiseVec3(glm::vec3& vec, YAML::Emitter& out) {
	out << YAML::Flow << YAML::BeginSeq << vec.x << vec.y << vec.z
		<< YAML::EndSeq;
}

static void serialiseQuat(glm::quat& quat, YAML::Emitter& out) {
	out << YAML::Flow << YAML::BeginSeq << quat.x << quat.y << quat.z << quat.w
		<< YAML::EndSeq;
}

static void serialiseEntity(Entity& entity, YAML::Emitter& out) {
	out << YAML::BeginMap;

	if (entity.hasComponent<TagComponent>()) {
		TagComponent& tag = entity.getComponent<TagComponent>();

		out << YAML::Key << "Tag";
		out << YAML::Value << tag.Tag;
	}

	if (entity.hasComponent<TransformComponent>()) {
		TransformComponent& transform =
			entity.getComponent<TransformComponent>();

		out << YAML::Key << "Transform";
		out << YAML::Value;
		out << YAML::BeginMap;
		out << YAML::Key << "Position";
		out << YAML::Value;
		serialiseVec3(transform.Position, out);
		out << YAML::Key << "Size";
		out << YAML::Value;
		serialiseVec3(transform.Size, out);
		out << YAML::Key << "Rotation";
		out << YAML::Value;
		serialiseQuat(transform.Rotation, out);
		out << YAML::EndMap;
	}

	if (entity.hasComponent<ColourComponent>()) {
		ColourComponent& colour = entity.getComponent<ColourComponent>();

		out << YAML::Key << "Colour";
		out << YAML::Value;
		serialiseVec3(colour.Colour, out);
	}

	if (entity.hasComponent<TextureComponent>()) {
		TextureComponent& texture = entity.getComponent<TextureComponent>();

		out << YAML::Key << "Texture";
		out << YAML::Value;
		out << YAML::BeginMap;
		out << YAML::Key << "Path";
		out << YAML::Value << texture.Path;
		out << YAML::Key << "Tile Count";
		out << YAML::Value << texture.TileCount;
		out << YAML::EndMap;
	}

	if (entity.hasComponent<ModelComponent>()) {
		ModelComponent& model = entity.getComponent<ModelComponent>();

		out << YAML::Key << "Model";
		out << YAML::Value;
		out << YAML::BeginMap;
		out << YAML::Key << "Path";
		out << YAML::Value << model.Path;
		out << YAML::EndMap;
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

void operator>>(const YAML::Node& node, glm::vec3& vector) {
	vector.x = node[0].as<float>();
	vector.y = node[1].as<float>();
	vector.z = node[2].as<float>();
}

void operator>>(const YAML::Node& node, glm::quat& quat) {
	quat.x = node[0].as<float>();
	quat.y = node[1].as<float>();
	quat.z = node[2].as<float>();
	quat.w = node[3].as<float>();
}

void operator>>(const YAML::Node& node, TagComponent& tagComponent) {
	tagComponent.Tag = node.as<std::string>();
}

void operator>>(const YAML::Node& node, ColourComponent& colourComponent) {
	node >> colourComponent.Colour;
}

void operator>>(const YAML::Node& node, TextureComponent& textureComponent) {
	textureComponent.Path = node["Path"].as<std::string>();
	textureComponent.TileCount = node["Tile Count"].as<float>();
}

void operator>>(const YAML::Node& node, ModelComponent& modelComponent) {
	modelComponent.Path = node["Path"].as<std::string>();
}

void operator>>(
	const YAML::Node& node, TransformComponent& transformComponent) {
	node["Position"] >> transformComponent.Position;
	node["Size"] >> transformComponent.Size;
	node["Rotation"] >> transformComponent.Rotation;
}

template <typename T>
void loadComponent(
	const YAML::Node& node, Entity& entity, const std::string& key) {
	if (YAML::Node componentNode = node[key]) {
		entity.addComponent<T>();
		T& component = entity.getComponent<T>();
		componentNode >> component;
	}
}

void operator>>(const YAML::Node& node, Entity& entity) {
	loadComponent<TagComponent>(node, entity, "Tag");
	loadComponent<TransformComponent>(node, entity, "Transform");
	loadComponent<TextureComponent>(node, entity, "Texture");
	loadComponent<ColourComponent>(node, entity, "Colour");
	loadComponent<ModelComponent>(node, entity, "Model");
}

void SceneExporter::load(const std::string& path) {
	std::ifstream input(path);
	YAML::Node document = YAML::Load(input);

	Scene& scene = Game::getInstance().getScene();
	scene.getRegistry().clear();

	for (unsigned entityIndex = 0; entityIndex < document.size();
		 entityIndex++) {

		Entity entity = scene.createEntity();
		document[entityIndex] >> entity;
	}
}