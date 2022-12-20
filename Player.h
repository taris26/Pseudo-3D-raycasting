#pragma once
#include "game.h"
class Player
{
public:
	int x, y, const playerWidth = 10, const playerHeight = 10, const speed = 5;
	double  langle, rangle, cangle = 4.245575, const fov = M_PI / 3, xpos = 500, ypos = 703;
	void move(double angle, Game& game);
};
