#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_timer.h>
#include <vector>
#include <set>
#include <queue>
#include <cmath>

class Render;
class Update;
class Player;
class Game;
#include "player.h"
#include "update.h"
#include "render.h"

using namespace std;
//sutra dan ce nam to biti posao
class Game
{
public:
	bool close;
	void init();
	void updateGame();
	void renderGame();
	void destroy();
	Player player;
	int sizex, sizey, width, height;
	SDL_Window* winGrid;
	SDL_Renderer* rendGrid;
	SDL_Window* winPov;
	SDL_Renderer* rendPov;
	vector<vector<bool>> wall;
private:
	Update update;
	Render render;
	
	void pollEvents();

};
