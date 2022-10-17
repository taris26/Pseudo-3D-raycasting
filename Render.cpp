#include "Render.h"

void Render::renderGrid(Game& game,Player& player) {
	
}
void Render::drawGrid(Game& game) {
	for (int i = 0; i < game.sizey; i++) {
		for (int j = 0; j < game.sizex; j++) {
			const SDL_Rect rect = { game.width * j, game.height * i, game.width, game.height };
			SDL_SetRenderDrawColor(game.rendGrid, 255, 255, 255, 255);
			SDL_RenderDrawRect(game.rendGrid, &rect);
			if (!game.wall[i][j]) {
				SDL_SetRenderDrawColor(game.rendGrid, 0, 0, 255, 255);
				SDL_RenderFillRect(game.rendGrid, &rect);
			}
		}
	}
}

double Render::ang(double x1, double y1, double x2, double y2) {
	if (x1 == x2) return 0;
	//double x1 = x1t, y1 = y1t, x2 = x2t, y2 = y2t;
	double  angle = atan(-(y2 - y1) / (x2 - x1));
	if (x2 < x1)  angle -= M_PI;
	while (angle < 0) angle += 2 * M_PI;
	return angle;
}
pair <int, int> Render::presek(double angle, Player& player, int i, int j, Game& game) {
	int tx = 0, ty = 0;
	int posj[] = { 0, j * game.width, (j + 1) * game.width };
	int posi[] = { 0, i * game.height, (i + 1) * game.height };
	int fj = (player.x < j) + 2 * (player.x > j);
	int fi = (player.y < i) + 2 * (player.y > i);
	if (fj) {
		int temp = -tan(angle) * (posj[fj] - player.xpos) + player.ypos;
		if (temp >= posi[1] - 1 && temp <= posi[2]) {
			ty = temp;
			tx = posj[fj];
			return { tx, ty };
		}
	}
	if (fi) {
		int temp = (player.ypos - posi[fi]) / tan(angle) + player.xpos;
		if (temp >= posj[1] - 1 && temp <= posj[2]) {
			tx = temp;
			ty = posi[fi];
		}
	}
	return { tx, ty };
}

void Render::triangle(Game& game, float x1, float y1, float x2, float y2, float x3, float y3) {
	
	int a2 = 255 * 180 / (/*0.2466019 * */sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) /*- 11.330097*/);
	int a3 = 255 * 180 / (/*0.2466019 * */sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1)) /*- 11.330097*/);
	a2 = min(a2, 255);
	a3 = min(a3, 255);
	
	const std::vector< SDL_Vertex > verts =
	{
		{ SDL_FPoint{ (float)x1, (float)y1 }, SDL_Color{ 204, 115, 12, 255 }, SDL_FPoint{ 0 }, },
		{ SDL_FPoint{ (float)x2, (float)y2 }, SDL_Color{ 204, 115, 12, (unsigned char)(a2) }, SDL_FPoint{ 0 }, },
		{ SDL_FPoint{ (float)x3, (float)y3 }, SDL_Color{ 204, 115, 12, (unsigned char)(a3) }, SDL_FPoint{ 0 }, },
	};
	SDL_RenderGeometry(game.rendGrid, nullptr, verts.data(), verts.size(), nullptr, 0);
}

void Render::trapistSir(SDL_Renderer* rend2, Player& player, int x2, int y2, int x3, int y3) {
	int x1 = player.xpos, y1=player.ypos;
	int disProj = 500 / tan(player.fov / 2);
	double a2 = player.cangle - ang(x1, y1, x2, y2), a3 = player.cangle - ang(x1, y1, x3, y3);
	int d2 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)) * cos(a2), d3 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1)) * cos(a3);
	int x2p = 500 + disProj * tan(a2), x3p = 500 + disProj * tan(a3);
	int height2 = 100 * disProj / d2, height3 = 100 * disProj / d3;
	x2p = max(0, x2p);
	x3p = max(0, x3p);
	x2p = min(x2p, 999);
	x3p = min(x3p, 999);
	SDL_SetRenderDrawColor(rend2, 255, 255, 255, 255);
	SDL_RenderDrawLine(rend2, x2p, 500 + height2 / 2, x2p, 500 - height2 / 2);
	SDL_RenderDrawLine(rend2, x3p, 500 + height3 / 2, x3p, 500 - height3 / 2);
	SDL_RenderDrawLine(rend2, x2p, 500 + height2 / 2, x3p, 500 + height3 / 2);
	SDL_RenderDrawLine(rend2, x2p, 500 - height2 / 2, x3p, 500 - height3 / 2);
}

void Render::bfs(Game& game, Player& player) {
	set<pair<double, double>> cones;
	cones.insert({ player.rangle, player.langle });
	queue <pair <int, int>> q;
	q.push({ player.x, player.y });
	vector<vector<bool>> visited(game.sizey, vector<bool>(game.sizex));
	vector<pair<int, int>> offset = { {0, 0}, {game.width , 0}, {0, game.height}, {game.width , game.height} };
	while (!q.empty()) {
		int i = q.front().second, j = q.front().first;
		q.pop();
		if (i < 0 || j < 0 || i >= game.sizey || j >= game.sizex || visited[i][j]) continue;
		visited[i][j] = 1;
		q.push({ j, i + 1 });
		q.push({ j, i - 1 });
		q.push({ j + 1, i });
		q.push({ j - 1, i });
		if (!game.wall[i][j]) continue;
		set <pair <double, int>> corners;
		for (int it = 0; it < 4; it++) {
			if (it == (i > player.y) + (i > player.y) + (j > player.x)) continue;
			if ((player.x == j || player.y == i) && it == (j >= player.x) + (i >= player.y) + (i >= player.y)) continue;
			double angle = ang(player.xpos, player.ypos, game.width * j + offset[it].first, game.height * i + offset[it].second);
			if (!corners.empty()) {
				pair <double, int> reper = *corners.rbegin();
				while (reper.first - angle >= M_PI) angle += 2 * M_PI;
				reper = *corners.rbegin();
				while (angle - reper.first >= M_PI) angle -= 2 * M_PI;
			}
			corners.insert({ angle, it });
		}
		auto iterator = corners.begin();
		double lcorner = corners.rbegin()->first, rcorner = iterator->first;
		int lit = corners.rbegin()->second, rit = iterator->second;
		iterator++;
		double mcorner = iterator->first;
		int mit = iterator->second;
		while ((int)lcorner / 2 / M_PI < (int)player.rangle / 2 / M_PI) {
			rcorner += 2 * M_PI;
			lcorner += 2 * M_PI;
			mcorner += 2 * M_PI;
		}
		bool flag = 0;
		if (rcorner >= player.langle || lcorner <= player.rangle) continue;
		vector<pair<double, double>> delq;
		vector<pair<double, double>> insq;
		vector<pair<int, int>> points(3);
		for (auto const& range : cones) {
			double lv = range.second, rv = range.first;
			if (rv >= lcorner) break;
			if (lv < rcorner) continue;
			flag = 1;
			delq.push_back(range);
			//SDL_SetRenderDrawColor(game.rendGrid, 0, 255, 0, 255);

			if (rcorner > rv) {
				insq.push_back({ rv, rcorner });
				points[0] = { j * game.width + offset[rit].first, i * game.height + offset[rit].second };
			}
			else {
				pair<int, int> t = presek(rv, player, i, j, game);
				points[0] = { t.first,t.second };
			}

			if (lcorner < lv) {
				insq.push_back({ lcorner, lv }); \
					points[2] = { j * game.width + offset[lit].first, i * game.height + offset[lit].second };
			}
			else {
				pair<int, int> t = presek(lv, player, i, j, game);
				points[2] = { t.first,t.second };
			}

			if (mcorner<lv && mcorner>rv) {
				points[1] = { j * game.width + offset[mit].first, i * game.height + offset[mit].second };
			}
		}

		if (points[1].first == 0) {
			points[1] = points[2];
		}
		if (!flag) continue;
		for (auto const& el : delq) cones.erase(el);
		for (auto const& el : insq) cones.insert(el);

		triangle(game, player.xpos, player.ypos, points[0].first, points[0].second, points[1].first, points[1].second);
		trapistSir(game.rendPov,player, points[0].first, points[0].second, points[1].first, points[1].second);
		if (points[1] != points[2]) {
			triangle(game, player.xpos, player.ypos, points[1].first, points[1].second, points[2].first, points[2].second);
			trapistSir(game.rendPov, player, points[1].first, points[1].second, points[2].first, points[2].second);
		}
	}
}