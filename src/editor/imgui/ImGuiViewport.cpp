#include "ImGuiViewport.h"

#include "Game.h"

using namespace MattEngine::ImGuiCustom;

void Viewport::render(bool focusOnHover) {
	Game& game = Game::getInstance();

	ImGui::Begin("Game", NULL, ImGuiWindowFlags_NoScrollbar);
	ImVec2 screenSize = ImGui::GetContentRegionAvail();
	Framebuffer* framebuffer = game.getFramebuffer();
	Framebuffer* framebufferMultisampled = game.getFramebufferMultisampled();

	ImGui::Image(
		ImTextureID((void*)(uintptr_t)framebuffer->getColourTextureId()),
		ImVec2(screenSize.x, screenSize.y), ImVec2(0, 1), ImVec2(1, 0));

	if (screenSize.x != m_viewportSize.x || screenSize.y != m_viewportSize.y) {
		m_viewportSize = screenSize;
		game.resize({screenSize.x, screenSize.y});
	}

	if (focusOnHover) {
		Window::getInstance().setFocused(ImGui::IsWindowHovered());
	}

	ImGui::End();
}