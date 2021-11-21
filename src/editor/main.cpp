#include "EditorGame.h"

class Editor : public MattEngine::EditorGame {
public:
	Editor(MattEngine::Window& window) : MattEngine::EditorGame(window) {}

	void onInit() override {
		EditorGame::onInit();
	}
};

int main(int argc, char** argv) {
	MattEngine::Window window(1280, 720, "MattEngine");
	Editor(window).start();
	return 0;
}