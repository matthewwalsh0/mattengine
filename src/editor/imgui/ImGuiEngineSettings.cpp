#include "ImGuiEngineSettings.h"

#include "Game.h"
#include "PostProcessingLayer.h"

using namespace MattEngine::ImGuiCustom;

void EngineSettings::render() {
	ImGui::Begin("Engine", NULL, ImGuiWindowFlags_NoScrollbar);

	Game& game = Game::getInstance();

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
		ImGui::Text("Game");
		ImGui::Spacing();
		m_perspectiveCamera.render(game.getCamera());
	}

	if (ImGui::CollapsingHeader("Shadows")) {
		ShadowLayer& shadows = game.getShadows();

		ImGui::Checkbox("Enabled", &shadows.Enabled);

		if (shadows.Enabled) {
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
}