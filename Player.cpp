#include "Player.h"

void Player::move(double angle, Game& game) {
	pair <int, int> offsets[] = { {-playerWidth / 2, -playerHeight / 2}, {playerWidth / 2, -playerHeight / 2}, {-playerWidth / 2, playerHeight / 2}, {playerWidth / 2, playerHeight / 2} };
	double xtemp = xpos, ytemp = ypos;
	for (int i = 1; i <= speed; i++) {
		bool flagx = 0, flagy = 0;
		xtemp += cos(angle);
		for (const pair<int, int>& offset : offsets) {
			if (game.wall[(ytemp + offset.second) / game.height][(xtemp + offset.first) / game.width]) {
				xtemp -= cos(angle);
				flagx = 1;
				break;
			}
		}
		ytemp -= sin(angle);
		for (const pair<int, int>& offset : offsets) {
			if (game.wall[(ytemp + offset.second) / game.height][(xtemp + offset.first) / game.width]) {
				ytemp += sin(angle);
				flagy = 1;
				break;
			}
		}
		if (flagx && flagy) break;
	}
	xpos = xtemp;
	ypos = ytemp;
	x = xpos / game.width;
	y = ypos / game.height;
}