#include "Player.h"

void Player::move(double angle, Game& game) {
	pair <int, int> offsets[] = { {-playerWidth / 2, -playerHeight / 2}, {playerWidth / 2, -playerHeight / 2}, {-playerWidth / 2, playerHeight / 2}, {playerWidth / 2, playerHeight / 2} };
	double xtemp = xpos, ytemp = ypos;
	for (int i = 1; i <= speed; i++) {
		xtemp += cos(angle);
		ytemp -= sin(angle);
		bool flag = 0;
		for (const pair<int, int>& offset : offsets) {
			if (game.wall[(ytemp + offset.second) / game.height][(xtemp + offset.first) / game.width]) {
				xtemp -= cos(angle);
				ytemp += sin(angle);
				flag = 1;
				break;
			}
		}
		if (flag) break;
	}
	xpos = xtemp;
	ypos = ytemp;
	x = xpos / game.width;
	y = ypos / game.height;
}