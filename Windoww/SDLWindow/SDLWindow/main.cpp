#define SDL_MAIN_HANDLED
#include "Game.h"

Window *game = nullptr;

int main(int argc, char **argv) {
	SDL_SetMainReady();
	game = new Window();
	game->init("Game", false);
	while (game->running()) {

		game->update();
		game->HandleEvent();
		game->Tetris();
		game->render();
		
	}

	game->clean();
	return 0;
}
