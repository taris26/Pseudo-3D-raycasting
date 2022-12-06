#include "Game.h"

void Game::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
	}
	winGrid = SDL_CreateWindow("DOOM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
	Uint32 render_flags = SDL_RENDERER_ACCELERATED;
	rendGrid = SDL_CreateRenderer(winGrid, -1, render_flags);
	SDL_SetRenderDrawBlendMode(rendGrid, SDL_BLENDMODE_BLEND);
	winPov = SDL_CreateWindow("First person", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
	rendPov = SDL_CreateRenderer(winPov, -1, render_flags);

	player = new Player();

	player->langle = player->cangle + player->fov / 2;
	player->rangle = player->cangle - player->fov / 2;
	wall =
	{ {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
	{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1},
	{1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1},
	{1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
	{1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 1},
	{1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1},
	{1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0, 1},
	{1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1},
	{1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1} };
	sizex = wall[0].size();
	sizey = wall.size();
	width = 1000 / sizex;
	height = 1000 / sizey;
	player->x = player->xpos / width;
	player->y = player->ypos / height;
	render = new Render();
	update = new Update();
	
}

void Game::updateGame() {
	pollEvents();

}
void Game::renderGame() {
	SDL_RenderClear(rendGrid);
	SDL_RenderClear(rendPov);
	
	render->drawGrid(*this);

	const SDL_Rect rect = {
		player->xpos - player->playerWidth / 2, player->ypos - player->playerHeight / 2, player->playerWidth, player->playerHeight
	};
	SDL_SetRenderDrawColor(rendGrid, 255, 0, 0, 255);
	SDL_RenderDrawRect(rendGrid, &rect);
	SDL_RenderFillRect(rendGrid, &rect);

	render->bfs(*this, *player);

	SDL_SetRenderDrawColor(rendGrid, 0, 0, 0, 255);
	SDL_SetRenderDrawColor(rendPov, 0, 0, 0, 255);
	SDL_RenderPresent(rendGrid);
	SDL_RenderPresent(rendPov);
	SDL_Delay(1000 / 60); 
}

void Game::pollEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {

		case SDL_QUIT:
			close = 1;
			break;

		case SDL_WINDOWEVENT:
			if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
				close = 1;
				break;
			}

		case SDL_KEYDOWN:
			switch (event.key.keysym.scancode) {
			case SDL_SCANCODE_P:
				close = 1;
				break;
			case SDL_SCANCODE_W:
				player->move(player->cangle, *this);
				break;
			case SDL_SCANCODE_A:
				player->move(player->cangle + M_PI / 2, *this);
				break;
			case SDL_SCANCODE_S:
				player->move(player->cangle + M_PI, *this);
				break;
			case SDL_SCANCODE_D:
				player->move(player->cangle - M_PI / 2, *this);
				break;
			case SDL_SCANCODE_RIGHT:
				player->langle -= 0.05;
				player->rangle -= 0.05;
				if (player->rangle < 0) {
					player->langle += 2 * M_PI;
					player->rangle += 2 * M_PI;
				}
				player->cangle = (player->rangle + player->langle) / 2;
				break;
			case SDL_SCANCODE_LEFT:
				player->langle += 0.05;
				player->rangle += 0.05;
				if (player->langle > 4 * M_PI) {
					player->langle -= 2 * M_PI;
					player->rangle -= 2 * M_PI;
				}
				player->cangle = (player->rangle + player->langle) / 2;
				break;
			case SDL_SCANCODE_I:
				cout << "player->langle: " << player->langle << ' ' << "player->cangle: " << player->cangle << ' ' << "player->rangle: " << player->rangle << endl;
				cout << "player->xpos: " << player->xpos << ' ' << "player->ypos: " << player->ypos << endl;
				break;
			default:
				break;
			}
		}
	}
}
void Game::destroy() {
	SDL_DestroyRenderer(rendPov);
	SDL_DestroyWindow(winPov);

	SDL_DestroyRenderer(rendGrid);
	SDL_DestroyWindow(winGrid);
	SDL_Quit();
}