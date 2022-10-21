#pragma once
#include "game.h"
class Player
{
public:
	int x, y, playerWidth = 10, playerHeight = 10, speed = 10;
	double  langle, rangle, cangle = 3 * M_PI / 2, fov = M_PI / 3, xpos, ypos;
	void move(double angle, Game& game);
};
