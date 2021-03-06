#include "ImGuiEngineSettings.h"

#include "Game.h"
#include "Log.h"
#include "PostProcessingLayer.h"

using namespace MattEngine::ImGuiCustom;

void EngineSettings::render() {
	ImGui::Begin("Engine", NULL, ImGuiWindowFlags_NoScrollbar);

	Game& game = Game::getInstance();
	ShadowLayer& shadows = game.getShadows();

	if (ImGui::CollapsingHeader("Post-Processing")) {
		ImGui::Text("Anti-Aliasing");
		ImGui::Spacing();

		Framebuffer* framebufferMultisampled =
			game.getFramebufferMultisampled();

		PostProcessingLayer& postProcessing = game.getPostProcessing();

		int samples = game.getFramebufferMultisampled()->getSamples();
		bool samplesChanged = ImGui::SliderInt("MSAA", &samples, 0, 16);

		if (samplesChanged) {
			framebufferMultisampled->setSamples(samples);
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Text("Tone Mapping");
		ImGui::Checkbox(
			"Enabled##ToneMappingEnabled", &postProcessing.ToneMapping);

		if (postProcessing.ToneMapping) {
			ImGui::DragFloat(
				"Exposure", &postProcessing.Exposure, 0.01f, 0.01f, 20.0f);
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Text("Bloom");
		ImGui::Spacing();
		ImGui::Checkbox("Enabled##BloomEnabled", &postProcessing.Bloom);

		if (postProcessing.Bloom) {
			ImGui::SliderFloat(
				"Threshold", &postProcessing.BloomThreshold, 0.01, 1.0);

			if (ImGui::SliderInt(
					"Passes", (int*)&postProcessing.BloomBlurPasses, 1, 100)) {
				game.resize(game.getFramebuffer()->getSize());
			}

			if (ImGui::SliderFloat(
					"Downsample", &postProcessing.BloomBlurSize, 0.01, 1.0)) {
				game.resize(game.getFramebuffer()->getSize());
			}
		}

		ImGui::Spacing();
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Text("Effects");
		ImGui::Spacing();
		ImGui::Checkbox("Grayscale", &postProcessing.Grayscale);
		ImGui::Checkbox("Invert", &postProcessing.Invert);
		ImGui::Spacing();
	}

	if (ImGui::CollapsingHeader("Physics")) {
		ImGui::Checkbox("Render Objects", &game.RenderPhysicsObjects);
	}

	if (ImGui::CollapsingHeader("Camera")) {
		ImGui::Checkbox("Render Camera Bounds", &RenderCameraBounds);
		ImGui::Text("Game");
		ImGui::Spacing();
		m_perspectiveCamera.render(game.getCamera());
	}

	if (ImGui::CollapsingHeader("Shadows")) {
		ImGui::Checkbox("Enabled", &shadows.Enabled);

		if (shadows.Enabled) {
			ImGui::Checkbox(
				"Render Shadow Camera Bounds", &RenderShadowCameraBounds);

			ImGui::Checkbox(
				"Cascade Indicators", &shadows.CascadeIndicatorsEnabled);

			std::vector<unsigned int>& cascadeSizes = shadows.DepthMapSizes;
			std::vector<float>& cascadeFarPlanes = shadows.DepthMapFarPlanes;
			unsigned int* depthMapCount = &shadows.DepthMapCount;

			for (int i = 0; i < *depthMapCount; i++) {
				ImGui::PushID(i);
				ImGui::Text("Cascade %d", i + 1);
				ImGui::Spacing();

				if (ImGui::DragInt("Size", (int*)&cascadeSizes.at(i), 1.0f, 1,
						1024 * 10)) {
					shadows.onInit();
				}

				ImGui::DragFloat(
					"Far Plane", &cascadeFarPlanes.at(i), 1.0f, 0.1f, 1000.0f);
				ImGui::Spacing();
				ImGui::Separator();
				ImGui::Spacing();
				ImGui::PopID();
			}
		}
	}

	ImGui::End();

	ImGui::Begin("Shadow Maps", NULL, ImGuiWindowFlags_NoScrollbar);
	ImVec2 screenSize = ImGui::GetContentRegionAvail();
	bool portrait = screenSize.y > screenSize.x;
	float aspectRatio = game.getFramebuffer()->getHeight() /
						(float)game.getFramebuffer()->getWidth();

	float width = portrait ? screenSize.y / 4 / aspectRatio : screenSize.x / 4;
	float height = portrait ? screenSize.y / 4 : screenSize.x / 4 * aspectRatio;

	if (portrait && width > screenSize.x) {
		width = screenSize.x;
		height = width * aspectRatio;
	}

	if (!portrait && height > screenSize.y) {
		height = screenSize.y;
		width = height / aspectRatio;
	}

	for (int depthMapIndex = 0; depthMapIndex < 4; depthMapIndex++) {
		ImGui::Image(
			ImTextureID((void*)(uintptr_t)shadows.DepthMaps[depthMapIndex]
							.getColourTextureId()),
			ImVec2(width, height), ImVec2(0, 1), ImVec2(1, 0));

		if (!portrait) {
			ImGui::SameLine();
		}
	}

	ImGui::End();
}