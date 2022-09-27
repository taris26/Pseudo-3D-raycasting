#include <iostream>
#include <SDL.h>
#include <SDL_timer.h>
#include <vector>
#include <set>
#include <queue>
#include <cmath>

using namespace std;

int sizex, sizey, width, height, tx, ty;
double langle = 10.9978, rangle = 9.22699;

double ang(int x1, int y1, int x2, int y2) {
	return atan((y2 - y1) * 1.0 / (x2 - x1));
}

void presek(double angle, int x, int y, vector<vector<bool>>& wall) {
	int i = y / height, j = x / width;
	int offx = 0, offy = 0;
	int sign[2] = { 1, -1 };
	while (!wall[i][j]) {
		if (abs(tan(angle)) < 1) {
			offx += sign[cos(angle) < 0];
			offy = -offx * tan(angle);
		}
		else {
			offy += sign[sin(angle) > 0];
			offx = -offy / tan(angle);
		}
		i = (y + offy) / height;
		j = (x + offx) / width;
	}
	tx = x + offx;
	ty = y + offy;
}

//void vision(int x, int y, vector<vector<bool>>& wall, set<pair<double, double>>& cones) {
//	int flag = 1;
//	int offx = 1, offy = 1;
//	while (flag) {
//		flag = 0;
//		if (x + offx < sizex) {
//			for (int i = 0; i < sizex; i++) {
//				if (!wall[x + offx][y]) continue;
//				double angle = ang() += (int) (*cones.begin().first) / 2 / M_PI;
//				if (angle > *cones.begin().first && angle > *cones.end().second) {
//
//				}
//			}
//		}
//	}
//}

void bfs(int x, int y, vector<vector<bool>>& wall, set<pair<double, double>>& cones) {
	queue <pair <int, int>> q;
	q.push({ x, y });
	vector<vector<bool>> visited(sizey, vector<bool>(sizex));
	vector<pair<int, int>> offset = { {0, 0}, {width, 0}, {0, height}, {width, height} };
	while (!q.empty()) {
		int i = q.front().second, j = q.front().first;
		q.pop();
		if (i < 0 || j < 0 || i >= sizey || j >= sizex || visited[i][j]) continue;
		visited[i][j] = 1;
		q.push({ i, j + 1 });
		q.push({ i, j - 1 });
		q.push({ i + 1, j });
		q.push({ i - 1, j });
		if (!wall[i][j]) continue; //i prolazi imaju susede, ide se van granica grida
		for (int it = 0; it < 4; it++) {
			if (it == i > y + i > y + j > x) continue;
			double angle = ang(width * x + width / 2, height * y + height / 2, width * j + offset[it].first, height * i + offset[it].second);
			angle += rangle / 2 / M_PI;
			if (angle<rangle || angle>langle) continue;
			cout << j << ' ' << i << endl;
			for (auto const& range : cones) {
				if (angle > range.first && angle < range.second) {
					cones.erase({ range.first, range.second });
					cones.insert({ range.first, angle });
					cones.insert({ angle, range.second });
					break;
				}
			}
		}
	}
}

int main(int argc, char* argv[])
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
	}
	SDL_Window* win = SDL_CreateWindow("DOOM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
	Uint32 render_flags = SDL_RENDERER_ACCELERATED;
	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

	int x = 4, y = 1;
	int close = 0;
	vector<vector<bool>> wall = { {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 1, 0, 1, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 1, 1, 0, 1, 1, 0, 0, 1},
		{1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1} };
	sizex = wall[0].size();
	sizey = wall.size();
	width = 1000 / sizex;
	height = 1000 / sizey;
 
	while (!close) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
 
			case SDL_QUIT:
				close = 1;
				break;
 
			case SDL_KEYDOWN:
				switch (event.key.keysym.scancode) {
				case SDL_SCANCODE_W:
				//case SDL_SCANCODE_UP:
					if (y > 0 && !wall[y - 1][x])
						y -= 1;
					break;
				case SDL_SCANCODE_A:
					if (x > 0 && !wall[y][x - 1])
						x -= 1;
					break;
				case SDL_SCANCODE_S:
				//case SDL_SCANCODE_DOWN:
					if (y < sizey - 1 && !wall[y + 1][x])
						y += 1;
					break;
				case SDL_SCANCODE_D:
					if (x < sizex - 1 && !wall[y][x + 1])
						x += 1;
					break;
				case SDL_SCANCODE_RIGHT:
					langle -= 0.1;
					rangle -= 0.1;
					//cout << langle << ' ' << rangle << endl;
					break;
				case SDL_SCANCODE_LEFT:
					langle += 0.1;
					rangle += 0.1;
					//cout << langle << ' ' << rangle << endl;
					break;
				default:
					break;
				}
			}
		}
		SDL_RenderClear(rend);
		//SDL_RenderCopy(rend, tex, NULL, &dest);

		for (int i = 0; i < sizey; i++) {
			for (int j = 0; j < sizex; j++) {
				const SDL_Rect rect = { 1000 / sizex * j, 1000 / sizey * i, 1000 / sizex, 1000 / sizey };
				SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
				SDL_RenderDrawRect(rend, &rect);
				if (!wall[i][j]) {
					SDL_SetRenderDrawColor(rend, 0, 0, 255, 255);
					SDL_RenderFillRect(rend, &rect);
				}
			}
		}

		const SDL_Rect rect = { 1000 / sizex * x, 1000 / sizey * y, 1000 / sizex, 1000 / sizey };
		SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
		SDL_RenderDrawRect(rend, &rect);
		SDL_RenderFillRect(rend, &rect);

		SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
		presek(langle, width* x + width / 2, height* y + height / 2, wall);
		//cout << "||" << tx << ' ' << ty << ' ';
		SDL_RenderDrawLine(rend, width * x + width / 2, height * y + height / 2, tx, ty);
		presek(rangle, width * x + width / 2, height * y + height / 2, wall);
		//cout << "||" << tx << ' ' << ty << ' ';
		SDL_RenderDrawLine(rend, width * x + width / 2, height * y + height / 2, tx, ty);

		set<pair<double, double>> cones;
		cones.insert({ rangle, langle });
		bfs(x, y, wall, cones);

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderPresent(rend);
		SDL_Delay(1000 / 60);
	}
 
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
 
	return 0;
}