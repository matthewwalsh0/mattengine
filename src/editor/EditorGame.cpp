#include "EditorGame.h"

#include "EditorLayer.h"

using namespace MattEngine;

EditorGame::EditorGame(Window& window) : Game(window) {}

void EditorGame::onInit() { addLayer(new EditorLayer()); }