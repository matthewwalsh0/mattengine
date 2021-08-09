#include "ShadowLayer.h"

#include "ColourComponent.h"
#include "Game.h"
#include "ModelComponent.h"
#include "ModelStore.h"
#include "PerspectiveCamera.h"
#include "PointLightComponent.h"
#include "Renderer.h"
#include "Scene.h"
#include "TextureComponent.h"

#include <glm/gtx/color_space.hpp>

namespace MattEngine {

void ShadowLayer::onInit() {
	m_depthMaps.clear();

	for (int depthMapIndex = 0; depthMapIndex < DepthMapCount;
		 depthMapIndex++) {
		int size = DepthMapSizes[depthMapIndex];
		Framebuffer depthMap(size, size, true, true);
		m_depthMaps.push_back(depthMap);
	}
}

void ShadowLayer::onBeforeRender() {
	Renderer& renderer = Renderer::getInstance();
	Game& game = Game::getInstance();
	Scene& scene = game.getScene();
	Camera& camera = game.getCamera();

	glm::vec3 lightPosition = {0.0f, 10.0f, 0.0f};
	Entity light = scene.getEntity("Light");

	if (light && light.hasComponent<PointLightComponent>()) {
		lightPosition =
			light.getComponent<PointLightComponent>().Light.Position;
	}

	for (int depthMapIndex = 0; depthMapIndex < DepthMapCount;
		 depthMapIndex++) {
		Framebuffer& depthMap = m_depthMaps[depthMapIndex];
		depthMap.bind();

		Texture(depthMap.getDepthTextureId()).bind(depthMapIndex + 1);

		renderer.setViewport(
			{0.0f, 0.0f}, {depthMap.getWidth(), depthMap.getHeight()});
		renderer.clear({1.0f, 1.0f, 1.0f});

		if (Enabled) {
			float nearPlane = 0.01f;
			float farPlane = DepthMapFarPlanes[depthMapIndex];

			PerspectiveCamera gameCamera(nearPlane, farPlane);
			gameCamera.setPosition(camera.getPosition());
			gameCamera.setRotation(((PerspectiveCamera&)camera).getRotation());

			OrthoCamera shadowCamera =
				gameCamera.getBoundingOrtho(lightPosition, scene);

			glm::vec3 cascadeIndicatorColour = {1.0f, 1.0f, 1.0f};

			if (CascadeIndicatorsEnabled) {
				glm::vec3 cascadeIndicatorColourHSV = {
					(360 / DepthMapCount) * (depthMapIndex + 1), 1.0, 1.0};

				cascadeIndicatorColour =
					rgbColor(cascadeIndicatorColourHSV) * 0.2f + 0.8f;
			}

			renderer.beginShadowFrame(camera, shadowCamera, depthMapIndex,
				farPlane, cascadeIndicatorColour);

			glCullFace(GL_FRONT);

			scene.getRegistry()
				.view<TransformComponent, ColourComponent, TextureComponent,
					TagComponent>()
				.each([&](auto rawEntity, TransformComponent& transform,
						  ColourComponent& colour, TextureComponent& texture,
						  TagComponent& tag) {
					DrawCubeRequest request;
					request.Position = transform.Position;
					request.Size = transform.Size;
					request.Rotation = transform.Rotation;
					request.DepthOnly = true;

					if (tag.Tag != "Floor") {
						renderer.drawCube(request);
					}
				});

			scene.getRegistry().view<TransformComponent, ModelComponent>().each(
				[&](auto rawEntity, TransformComponent& transform,
					ModelComponent& model) {
					Model& modelReference =
						ModelStore::getInstance().getModel(model.Path);

					DrawModelRequest request(modelReference);
					request.Position = transform.Position;
					request.Size = transform.Size;
					request.Rotation = transform.Rotation;
					request.DepthOnly = true;

					renderer.drawModel(request);
				});

			glCullFace(GL_BACK);
		}

		depthMap.unbind();
	}
}
} // namespace MattEngine