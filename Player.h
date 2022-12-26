#pragma once
#include "game.h"
class Player
{
public:
	int x = 0, y = 0, cameraLevel = 500;
	const int playerWidth = 10, playerHeight = 10, speed = 5;
	double  langle = 0, rangle = 0, cangle = 9.64513, xpos = 602.488, ypos = 82.8019;
	const double  fov = M_PI / 3;
	void move(double angle, Game& game);
};
