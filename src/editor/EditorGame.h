#ifndef EDITOR_GAME_H
#define EDITOR_GAME_H

#include "Game.h"

namespace MattEngine {

class EditorGame : public Game {
public:
	EditorGame(Window& window);

	void onInit() override;
};

} // namespace MattEngine

#endif
