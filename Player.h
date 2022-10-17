#pragma once
#include "game.h"
class Player
{
public:
	int x, y, xpos, ypos, playerWidth = 10, playerHeight = 10, speed = 10;
	double  langle, rangle, cangle, fov;
	void move(double angle, Game& game);
};

