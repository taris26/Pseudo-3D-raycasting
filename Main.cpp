#include "Game.h"

int main(int argc, char* argv[]) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
	}
	Game game;
	game.init();
	while (!game.close) {
		game.renderGame();
		game.updateGame();
	}
	game.destroy();
	return 0;
}