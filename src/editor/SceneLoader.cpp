#include "SceneLoader.h"

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

// Types

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

void operator>>(const YAML::Node& node, PointLight& pointLight) {
	node["Position"] >> pointLight.Position;
	node["Ambient"] >> pointLight.Ambient;
	node["Diffuse"] >> pointLight.Diffuse;
	node["Specular"] >> pointLight.Specular;

	pointLight.Constant = node["Constant"].as<float>();
	pointLight.Linear = node["Linear"].as<float>();
	pointLight.Quadratic = node["Quadratic"].as<float>();
}

// Components

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

void operator>>(
	const YAML::Node& node, PointLightComponent& pointLightComponent) {
	node >> pointLightComponent.Light;
}

template <typename T>
static void loadComponent(
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
	loadComponent<PointLightComponent>(node, entity, "Point Light");
}

std::shared_ptr<Scene> SceneLoader::load(const std::string& path) {
	std::ifstream input(path);
	YAML::Node document = YAML::Load(input);

	auto scene = std::make_shared<Scene>();

	for (unsigned entityIndex = 0; entityIndex < document.size();
		 entityIndex++) {

		Entity entity = scene->createEntity();
		document[entityIndex] >> entity;
	}

	return scene;
}