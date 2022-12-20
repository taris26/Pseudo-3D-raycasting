#pragma once
#include <iostream>
#include <SDL.h>
#include <SDL_timer.h>
#include <SDL_ttf.h>
#include <vector>
#include <set>
#include <queue>
#include <cmath>
#include <string>
#include <sstream>

class Render;
class Update;
class Player;
class Game;
#include "player.h"


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
	Player* player;
	int sizex, sizey, width, height;
	SDL_Window* winPov;
	SDL_Renderer* rendPov;
	vector<vector<bool>> wall;
private:
	Update* update;
	Render* render;
	
	void pollEvents();

};

#include "update.h"
#include "render.h"
