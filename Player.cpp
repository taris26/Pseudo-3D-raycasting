#include "Player.h"

void Player::move(double angle, Game& game) {
	int sign[] = { -1, 1 };
	int yoff = playerHeight / 2 * sign[sin(angle) < 0], xoff = playerWidth / 2 * sign[cos(angle) > 0];
	int xtemp = xpos + speed * cos(angle), ytemp = ypos - speed * sin(angle);
	if (xtemp < playerWidth || ytemp < playerHeight || xtemp >= 1000 - playerWidth || 
		ytemp >= 1000 - playerHeight || game.wall[(ytemp + yoff) / game.height][(xtemp + xoff) / game.width])
		return;
	xpos = xtemp;
	ypos = ytemp;
	x = xpos / game.width;
	y = ypos / game.height;
}