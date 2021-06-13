#include "ShadowLayer.h"

#include "ColourComponent.h"
#include "Game.h"
#include "ModelComponent.h"
#include "PerspectiveCamera.h"
#include "Renderer.h"
#include "Scene.h"
#include "TextureComponent.h"

namespace MattEngine {

void ShadowLayer::onInit() {
	for (int depthMapIndex = 0; depthMapIndex < m_depthMapCount;
		 depthMapIndex++) {
		int size = m_depthMapSizes[depthMapIndex];
		Framebuffer depthMap(size, size, true, true);
		m_depthMaps.push_back(depthMap);
	}
}

void ShadowLayer::onBeforeRender() {
	Renderer& renderer = Renderer::getInstance();
	Game& game = Game::getInstance();
	Scene& scene = *game.getScene();
	Camera& camera = game.getCurrentCamera();

	Entity light = scene.getEntity("Light");

	glm::vec3& lightPosition =
		light.getComponent<TransformComponent>().Position;

	for (int depthMapIndex = 0; depthMapIndex < m_depthMapCount;
		 depthMapIndex++) {
		Framebuffer& depthMap = m_depthMaps[depthMapIndex];
		depthMap.bind();

		Texture(depthMap.getDepthTextureId()).bind(depthMapIndex + 1);

		renderer.setViewport(
			{0.0f, 0.0f}, {depthMap.getWidth(), depthMap.getHeight()});
		renderer.clear({1.0f, 1.0f, 1.0f});

		float nearPlane = 0.01f;
		float farPlane = m_depthMapFarPlanes[depthMapIndex];

		PerspectiveCamera gameCamera(nearPlane, farPlane);
		gameCamera.setPosition(camera.getPosition());
		gameCamera.setRotation(((PerspectiveCamera&)camera).getRotation());

		OrthoCamera shadowCamera =
			gameCamera.getBoundingOrtho(lightPosition, scene);

		renderer.beginShadowFrame(
			camera, shadowCamera, farPlane, depthMapIndex);

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
				DrawModelRequest request(model.Model);
				request.Position = transform.Position;
				request.Size = transform.Size;
				request.Rotation = transform.Rotation;
				request.DepthOnly = true;

				renderer.drawModel(request);
			});

		glCullFace(GL_BACK);

		depthMap.unbind();
	}
}
} // namespace MattEngine