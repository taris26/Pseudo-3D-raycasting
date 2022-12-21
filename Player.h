#pragma once
#include "game.h"
class Player
{
public:
	int x = 0, y = 0;
	const int playerWidth = 10, playerHeight = 10, speed = 5;
	double  langle = 0, rangle = 0, cangle = 4.245575, xpos = 500, ypos = 700;
	const double  fov = M_PI / 3;
	void move(double angle, Game& game);
};
