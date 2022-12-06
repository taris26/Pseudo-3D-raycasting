#pragma once
#include "game.h"
class Render
{
public:
	void renderGrid(Game& game,Player& player);
	void drawGrid(Game& game);
	void bfs(Game& game, Player& player);
private:
	double ang(double x1, double y1, double x2, double y2);
	pair<int, int> presek(double angle, Player& player, int i, int j, Game& game);
	void triangle(Game& game, float x1, float y1, float x2, float y2, float x3, float y3);
	void trapistSir(SDL_Renderer* rend2, Player& player, int x2, int y2, int x3, int y3);
	int sign(int x);
};

