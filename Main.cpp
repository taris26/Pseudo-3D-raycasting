#include "Game.h"

//int sizex, sizey, width, height, xpos, ypos, x, y, playerWidth=10, playerHeight=10, speed=10;
//double langle, rangle, cangle = 3 * M_PI / 2, fov = M_PI / 3;
//
//double ang(double x1, double y1, double x2, double y2) {
//	if (x1 == x2) return 0;
//	//double x1 = x1t, y1 = y1t, x2 = x2t, y2 = y2t;
//	double  angle = atan(-(y2 - y1) / (x2 - x1));
//	if (x2 < x1)  angle -= M_PI;
//	while (angle < 0) angle += 2 * M_PI;
//	return angle;
//}
//
//void drawGrid(SDL_Renderer* rend, vector<vector<bool>>& wall) {
//	for (int i = 0; i < sizey; i++) {
//		for (int j = 0; j < sizex; j++) {
//			const SDL_Rect rect = { width * j, height * i, width, height };
//			SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
//			SDL_RenderDrawRect(rend, &rect);
//			if (!wall[i][j]) {
//				SDL_SetRenderDrawColor(rend, 0, 0, 255, 255);
//				SDL_RenderFillRect(rend, &rect);
//			}
//		}
//	}
//}
//
//void move(double angle, vector<vector<bool>>& wall) {
//	int sign[] = { -1, 1 };
//	int yoff = playerHeight / 2 * sign[sin(angle) < 0], xoff = playerWidth / 2 * sign[cos(angle) > 0];
//	int xtemp = xpos + speed * cos(angle), ytemp = ypos - speed * sin(angle);
//	if (xtemp < playerWidth || ytemp < playerHeight || xtemp >= 1000 - playerWidth || ytemp >= 1000 - playerHeight || wall[(ytemp + yoff) / height][(xtemp + xoff) / width])
//		return;
//	xpos = xtemp;
//	ypos = ytemp;
//	x = xpos / width;
//	y = ypos / width;
//}
//
//pair <int, int> presek(double angle, int x, int y, int i, int j, vector<vector<bool>>& wall) {
//	int tx = 0, ty = 0;
//	int posj[] = { 0, j * width, (j + 1) * width };
//	int posi[] = { 0, i * height, (i + 1) * height };
//	int fj = (x < j) + 2 * (x > j);
//	int fi = (y < i) + 2 * (y > i);
//	if (fj) {
//		int temp = -tan(angle) * (posj[fj] - xpos) + ypos;
//		if (temp >= posi[1]-1 && temp <= posi[2]) {
//			ty = temp;
//			tx = posj[fj];
//			return {tx, ty};
//		}
//	}
//	if (fi) {
//		int temp = (ypos - posi[fi]) / tan(angle) + xpos;
//		if (temp >= posj[1]-1 && temp <= posj[2]) {
//			tx = temp;
//			ty = posi[fi];
//		}
//	}
//	return { tx, ty };
//}
//
//void triangle(SDL_Renderer* rend, float x1, float y1, float x2, float y2, float x3, float y3) {
//	int a2 = 255 *180/ (/*0.2466019 * */sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) /*- 11.330097*/);
//	int a3 = 255 *180/ (/*0.2466019 * */sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1)) /*- 11.330097*/);
//	a2 = min(a2, 255);
//	a3 = min(a3, 255);
//	const std::vector< SDL_Vertex > verts =
//	{
//		{ SDL_FPoint{ (float) x1, (float) y1 }, SDL_Color{ 204, 115, 12, 255 }, SDL_FPoint{ 0 }, },
//		{ SDL_FPoint{ (float) x2, (float) y2 }, SDL_Color{ 204, 115, 12, (unsigned char)(a2) }, SDL_FPoint{ 0 }, },
//		{ SDL_FPoint{ (float) x3, (float) y3 }, SDL_Color{ 204, 115, 12, (unsigned char)(a3) }, SDL_FPoint{ 0 }, },
//	};
//	SDL_RenderGeometry(rend, nullptr, verts.data(), verts.size(), nullptr, 0);
//}
//
//void trapistSir(SDL_Renderer* rend2, int x1, int y1, int x2, int y2, int x3, int y3) {
//	int disProj = 500 / tan(fov / 2);
//	double a2 = cangle - ang(x1, y1, x2, y2), a3 = cangle - ang(x1, y1, x3, y3);
//	int d2 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) * cos(a2), d3 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1)) * cos(a3);
//	int x2p = 500 + disProj * tan(a2), x3p = 500 + disProj * tan(a3);
//	int height2 = 100 * disProj / d2, height3 = 100 * disProj / d3;
//	x2p = max(0, x2p);
//	x3p = max(0, x3p);
//	x2p = min(x2p, 999);
//	x3p = min(x3p, 999);
//	SDL_SetRenderDrawColor(rend2, 255, 255, 255, 255);
//	SDL_RenderDrawLine(rend2, x2p, 500 + height2 / 2, x2p, 500 - height2 / 2);
//	SDL_RenderDrawLine(rend2, x3p, 500 + height3 / 2, x3p, 500 - height3 / 2);
//	SDL_RenderDrawLine(rend2, x2p, 500 + height2 / 2, x3p, 500 + height3 / 2);
//	SDL_RenderDrawLine(rend2, x2p, 500 - height2 / 2, x3p, 500 - height3 / 2);
//}
//
//void bfs(int x, int y, vector<vector<bool>>& wall, SDL_Renderer* rend, SDL_Renderer* rend2) {
//	set<pair<double, double>> cones;
//	cones.insert({ rangle, langle });
//	queue <pair <int, int>> q;
//	q.push({ x, y });
//	vector<vector<bool>> visited(sizey, vector<bool>(sizex));
//	vector<pair<int, int>> offset = { {0, 0}, {width , 0}, {0, height}, {width , height} };
//	while (!q.empty()) {
//		int i = q.front().second, j = q.front().first;
//		q.pop();
//		if (i < 0 || j < 0 || i >= sizey || j >= sizex || visited[i][j]) continue;
//		visited[i][j] = 1;
//		q.push({ j, i + 1 });
//		q.push({ j, i - 1 });
//		q.push({ j + 1, i });
//		q.push({ j - 1, i });
//		if (!wall[i][j]) continue;
//		set <pair <double, int>> corners;
//		for (int it = 0; it < 4; it++) {
//			if (it == (i > y) + (i > y) + (j > x)) continue;
//			if ((x == j || y == i) && it == (j >= x) + (i >= y) + (i >= y)) continue;
//			double angle = ang(xpos, ypos, width * j + offset[it].first, height * i + offset[it].second);
//			if (!corners.empty()) {
//				pair <double, int> reper = *corners.rbegin();
//				while (reper.first - angle >= M_PI) angle += 2 * M_PI;
//				reper = *corners.rbegin();
//				while (angle - reper.first >= M_PI) angle -= 2 * M_PI;
//			}
//			corners.insert({ angle, it });
//		}
//		auto iterator = corners.begin();
//		double lcorner = corners.rbegin()->first, rcorner = iterator->first;
//		int lit = corners.rbegin()->second, rit = iterator->second;
//		iterator++;
//		double mcorner = iterator->first;
//		int mit = iterator->second;
//		while ((int) lcorner / 2 / M_PI < (int) rangle / 2 / M_PI) {
//			rcorner += 2 * M_PI;
//			lcorner += 2 * M_PI;
//			mcorner += 2 * M_PI;
//		}
//		bool flag = 0;
//		if (rcorner>=langle || lcorner<=rangle) continue;
//		vector<pair<double, double>> delq;
//		vector<pair<double, double>> insq;
//		vector<pair<int, int>> points(3);
//		for (auto const& range : cones) {
//			double lv = range.second, rv = range.first;
//			if (rv >= lcorner) break;
//			if (lv < rcorner) continue;
//			flag = 1;
//			delq.push_back(range);
//			SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
//
//			if (rcorner > rv) {
//				insq.push_back({ rv, rcorner });
//				points[0] = { j * width + offset[rit].first, i * height + offset[rit].second };
//			}
//			else {
//				pair<int, int> t = presek(rv, x, y, i, j, wall);
//				points[0] = { t.first,t.second };
//			}
//
//			if (lcorner < lv) {
//				insq.push_back({ lcorner, lv });\
//				points[2] = { j * width + offset[lit].first, i * height + offset[lit].second };
//			}
//			else {
//				pair<int, int> t = presek(lv, x, y, i, j, wall);
//				points[2] = { t.first,t.second };
//			}
//
//			if (mcorner<lv && mcorner>rv) {
//				points[1]= { j * width + offset[mit].first, i * height + offset[mit].second };
//			}
//		}
//
//		if (points[1].first == 0) {
//			points[1] = points[2];
//		}
//		if (!flag) continue;
//		for (auto const& el : delq) cones.erase(el);
//		for (auto const& el : insq) cones.insert(el);
//		//const SDL_Rect rect = { width * j, height * i, width, height };
//		//SDL_SetRenderDrawColor(rend, 100, 0, 100, 100);
//		//SDL_RenderDrawRect(rend, &rect);
//		//SDL_RenderFillRect(rend, &rect);
//		triangle(rend, xpos, ypos, points[0].first, points[0].second, points[1].first, points[1].second);
//		trapistSir(rend2, xpos, ypos, points[0].first, points[0].second, points[1].first, points[1].second);
//		if(points[1]!=points[2]) {
//			triangle(rend, xpos, ypos, points[1].first, points[1].second, points[2].first, points[2].second);
//			trapistSir(rend2, xpos, ypos, points[1].first, points[1].second, points[2].first, points[2].second);
//		}
//	}
//}
//
//int main(int argc, char* argv[]) {
//	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
//		printf("error initializing SDL: %s\n", SDL_GetError());
//	}
//	SDL_Window* win = SDL_CreateWindow("DOOM", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
//	Uint32 render_flags = SDL_RENDERER_ACCELERATED;
//	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
//	SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
//
//	SDL_Window* win2 = SDL_CreateWindow("First person", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 0);
//	SDL_Renderer* rend2 = SDL_CreateRenderer(win2, -1, render_flags);
//
//	x = 4; 
//	y = 1;
//	int close = 0;
//	langle = cangle + fov / 2;
//	rangle = cangle - fov / 2;
//	vector<vector<bool>> wall = 
//		{ {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//		{1, 0, 1, 0, 0, 0, 0, 0, 0, 1},
//		{1, 0, 0, 1, 0, 1, 0, 0, 0, 1},
//		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
//		{1, 0, 1, 1, 0, 1, 1, 0, 0, 1},
//		{1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
//		{1, 0, 0, 1, 0, 0, 0, 0, 0, 1},
//		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1} };
//	sizex = wall[0].size();
//	sizey = wall.size();
//	width = 1000 / sizex;
//	height = 1000 / sizey;
//	xpos = x * width + width / 2 + 1;
//	ypos = y * height + height / 2 + 1;
//
//	while (!close) {
//		SDL_Event event;
//		while (SDL_PollEvent(&event)) {
//			switch (event.type) {
// 
//			case SDL_QUIT:
//				close = 1;
//				break;
// 
//			case SDL_KEYDOWN:
//				switch (event.key.keysym.scancode) {
//				case SDL_SCANCODE_P:
//					close = 1;
//					break;
//				case SDL_SCANCODE_W:
//					move(cangle, wall);
//					break;
//				case SDL_SCANCODE_A:
//					move(cangle + M_PI / 2, wall);
//					break;
//				case SDL_SCANCODE_S:
//					move(cangle + M_PI, wall);
//					break;
//				case SDL_SCANCODE_D:
//					move(cangle - M_PI / 2, wall);
//					break;
//				case SDL_SCANCODE_RIGHT:
//					langle -= 0.05;
//					rangle -= 0.05;
//					if (rangle < 0) {
//						langle += 2 * M_PI;
//						rangle += 2 * M_PI;
//					}
//					cangle = (rangle + langle) / 2;
//					break;
//				case SDL_SCANCODE_LEFT:
//					langle += 0.05;
//					rangle += 0.05;
//					if (langle > 4 * M_PI) {
//						langle -= 2 * M_PI;
//						rangle -= 2 * M_PI;
//					}
//					cangle = (rangle + langle) / 2;
//					break;
//				case SDL_SCANCODE_I:
//					cout << "langle: " << langle << ' ' << "rangle: " << rangle << endl;
//					cout << "xpos: " << xpos << ' ' << "ypos: " << ypos << endl;
//					break;
//				default:
//					break;
//				}
//			}
//		}
//		SDL_RenderClear(rend);
//		SDL_RenderClear(rend2);
//
//		drawGrid(rend, wall);
//
//		const SDL_Rect rect = { xpos - playerWidth / 2, ypos - playerHeight / 2, playerWidth, playerHeight };
//		SDL_SetRenderDrawColor(rend, 255, 0, 0, 255);
//		SDL_RenderDrawRect(rend, &rect);
//		SDL_RenderFillRect(rend, &rect);
//
//		bfs(x, y, wall, rend, rend2);
//
//		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
//		SDL_SetRenderDrawColor(rend2, 0, 0, 0, 255);
//		SDL_RenderPresent(rend);
//		SDL_RenderPresent(rend2);
//		SDL_Delay(1000 / 60);
//	}
//
//	SDL_DestroyRenderer(rend2);
//	SDL_DestroyWindow(win2);
// 
//	SDL_DestroyRenderer(rend);
//	SDL_DestroyWindow(win);
//	SDL_Quit();
// 
//	return 0;
//}

int main(int argc, char* argv[]) {
	Game game;
	game.init();
	while (!game.close) {
		game.updateGame();
		game.renderGame();
	}
	game.destroy();
	return 0;
}