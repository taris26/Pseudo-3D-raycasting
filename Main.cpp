#include <iostream>
#include <SDL.h>
#include <SDL_timer.h>
#include <vector>
#include <set>
#include <queue>
#include <cmath>

using namespace std;

int sizex, sizey, width, height, tx, ty;
double langle = 9.8155, rangle = 8.04469;

double ang(int x1t, int y1t, int x2t, int y2t) {
	if (x1t == x2t) return 0;
	double x1 = x1t, y1 = y1t, x2 = x2t, y2 = y2t;
	double  angle = atan(-(y2 - y1) / (x2 - x1));
	if (x2 < x1)  angle -= M_PI;
	while (angle < 0) angle += 2 * M_PI;
	return angle;
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

void bfs(int x, int y, vector<vector<bool>>& wall, set<pair<double, double>>& cones, SDL_Renderer* rend) {
	queue <pair <int, int>> q;
	q.push({ x, y });
	vector<vector<bool>> visited(sizey, vector<bool>(sizex));
	vector<pair<int, int>> offset = { {0, 0}, {width , 0}, {0, height }, {width , height } };
	while (!q.empty()) {
		int i = q.front().second, j = q.front().first;
		q.pop();
		if (i < 0 || j < 0 || i >= sizey || j >= sizex || visited[i][j]) continue;
		visited[i][j] = 1;
		q.push({ j, i + 1 });
		q.push({ j, i - 1 });
		q.push({ j + 1, i });
		q.push({ j - 1, i });
		if (!wall[i][j]) continue;
		set <double> corners;
		for (int it = 0; it < 4; it++) {
			if (it == (i > y) + (i > y) + (j > x)) continue;
			double angle = ang(width * x + width / 2, height * y + height / 2, width * j + offset[it].first, height * i + offset[it].second);
			if (!corners.empty()) {
				double reper = *corners.rbegin();
				while (reper - angle >= M_PI) angle += 2 * M_PI;
				reper = *corners.begin();
				while (angle - reper >= M_PI) angle -= 2 * M_PI;
			}
			corners.insert(angle);
		}
		double lcorner = *(corners.rbegin()), rcorner = *(corners.begin());
		while ((int) lcorner / 2 / M_PI < (int) rangle / 2 / M_PI) {
			rcorner += 2 * M_PI;
			lcorner += 2 * M_PI;
		}
		bool flag = 0;
		if (rcorner>=langle || lcorner<=rangle) continue;
		vector<pair<double, double>> delq;
		vector<pair<double, double>> insq;
		for (auto const& range : cones) {
			double lv = range.second, rv = range.first;
			if (rv >= lcorner) break;
			if (lv < rcorner) continue;
			flag = 1;
			//cones.erase(range);
			delq.push_back(range);
			//if (rcorner > rv) cones.insert({ rv, rcorner });
			if (rcorner > rv) insq.push_back({ rv, rcorner });
			//if (lcorner < lv) cones.insert({ lcorner, lv });
			if (lcorner < lv) insq.push_back({ lcorner, lv });
		}
		//cout << j << ' ' << i << endl;
		if (!flag) continue;
		for (auto const& el : delq) cones.erase(el);
		for (auto const& el : insq) cones.insert(el);
		const SDL_Rect rect = { width * j, height * i, width, height };
		SDL_SetRenderDrawColor(rend, 100, 0, 100, 100);
		SDL_RenderDrawRect(rend, &rect);
		SDL_RenderFillRect(rend, &rect);
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

	int x = 2, y = 5;
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
					if (rangle < -0 * M_PI) {
						langle += 2 * M_PI;
						rangle += 2 * M_PI;
					}
					//cout << langle << ' ' << rangle << endl;
					break;
				case SDL_SCANCODE_LEFT:
					langle += 0.1;
					rangle += 0.1;
					if (langle > 4 * M_PI) {
						langle -= 2 * M_PI;
						rangle -= 2 * M_PI;
					}
					cout << langle << ' ' << rangle << endl;
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
		//cout << endl;
		cones.insert({ rangle, langle });
		bfs(x, y, wall, cones, rend);

		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderPresent(rend);
		SDL_Delay(1000 / 60);
	}
 
	SDL_DestroyRenderer(rend);
	SDL_DestroyWindow(win);
	SDL_Quit();
 
	return 0;
}