#include "Game.h"

int main(int argc, char* argv[]) {
	Game game;
	game.init();
	while (!game.close) {
		game.renderGame();
		game.updateGame();
	}
	game.destroy();
	return 0;
}