#include "Render.h"

void Render::renderGrid(Game& game,Player& player) {
	
}

int Render::sign(int x) {
	//signum function
	if (x == 0) return 0;
	else if (x > 0) return 1;
	return -1;
}
void Render::drawGrid(Game& game, Player& player) {

	//drawing 2d map
	for (int i = 0; i < game.sizey; i++) {
		for (int j = 0; j < game.sizex; j++) {
			if (game.wall[i][j]) {
				SDL_Rect rectMm = { game.width * j / scale, game.height * i / scale, game.width / scale, game.height / scale };
				SDL_SetRenderDrawColor(game.rendPov, 255, 255, 255, 255);
				SDL_RenderDrawRect(game.rendPov, &rectMm);
			}
		}
	}

	//drawing player
	const SDL_Rect rect = {
		(int) (player.xpos - player.playerWidth / 2) / scale, (int)(player.ypos - player.playerHeight / 2) / scale, (int)player.playerWidth / scale, (int)player.playerHeight / scale
	};
	SDL_SetRenderDrawColor(game.rendPov, 255, 0, 0, 255);
	SDL_RenderDrawRect(game.rendPov, &rect);
	SDL_RenderFillRect(game.rendPov, &rect);
}

double Render::ang(double x1, double y1, double x2, double y2) {
	//calculating angle between 2 points
	double angle;
	if (x1 == x2) angle = -(y2 - y1) / abs(y2 - y1) * M_PI / 2;
	else angle = atan(-(y2 - y1) / (x2 - x1));
	if (x2 < x1)  angle -= M_PI;
	while (angle < 0) angle += 2 * M_PI;
	return angle;
}
pair <int, int> Render::presek(double angle, Player& player, int i, int j, Game& game) {
	//finding coordinated of intersection between a line starting from player positiong under given angle and a wall
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

void Render::triangle(Game& game, float x1, float y1, float x2, float y2, float x3, float y3, SDL_Texture* tekstura) {
	//drawing triangle with given vertices
	
	SDL_SetRenderTarget(game.rendPov, tekstura);
	int a2 = (int) 255 * 180 / (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
	int a3 = (int) 255 * 180 / (sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1)));
	a2 = min(a2, 255);
	a3 = min(a3, 255);
	
	const std::vector< SDL_Vertex > verts =
	{
		{ SDL_FPoint{ (float)x1 / scale, (float)y1 / scale }, SDL_Color{ 255, 255, 255, 255 }, SDL_FPoint{ 0 }, },
		{ SDL_FPoint{ (float)x2 / scale, (float)y2 / scale }, SDL_Color{ 255, 255, 255, (unsigned char)(a2) }, SDL_FPoint{ 0 }, },
		{ SDL_FPoint{ (float)x3 / scale, (float)y3 / scale }, SDL_Color{ 255, 255, 255, (unsigned char)(a3) }, SDL_FPoint{ 0 }, },
	};
	SDL_RenderGeometry(game.rendPov, nullptr, verts.data(), verts.size(), nullptr, 0);
	SDL_SetRenderTarget(game.rendPov, NULL);
}

void Render::trapistSir(SDL_Renderer* rend2, Player& player, int x2, int y2, int x3, int y3) {
	//projecting simulated 3d space on 2d plane
	int x1 = player.xpos, y1=player.ypos;
	double disProj = 500 / tan(player.fov / 2);
	double a2 = player.cangle - ang(x1, y1, x2, y2), a3 = player.cangle - ang(x1, y1, x3, y3);
	double d2 = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)), d3 = sqrt((x3 - x1) * (x3 - x1) + (y3 - y1) * (y3 - y1));
	int x2p = 500 + disProj * tan(a2), x3p = 500 + disProj * tan(a3);
	x2p = max(x2p, -1);
	x3p = max(x3p, -1);
	x2p = min(x2p, 1000);
	x3p = min(x3p, 1000);
	int height2 = 50 * disProj / d2 / cos(a2), height3 = 50 * disProj / d3 / cos(a3);
	x2p = max(0, x2p);
	x3p = max(0, x3p);
	x2p = min(x2p, 999);
	x3p = min(x3p, 999);

	//drawing the plane
	SDL_SetRenderDrawColor(rend2, 255, 255, 255, 255);
	SDL_RenderDrawLine(rend2, x2p, 500 + height2 / 2, x2p, 500 - height2 / 2);
	SDL_RenderDrawLine(rend2, x3p, 500 + height3 / 2, x3p, 500 - height3 / 2);
	SDL_RenderDrawLine(rend2, x2p, 500 + height2 / 2, x3p, 500 + height3 / 2);
	SDL_RenderDrawLine(rend2, x2p, 500 - height2 / 2, x3p, 500 - height3 / 2);
}

void Render::bfs(Game& game, Player& player) {


	SDL_Texture* tekstura = SDL_CreateTexture(game.rendPov, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, 200, 200);
	SDL_SetRenderTarget(game.rendPov, tekstura);
	SDL_SetRenderDrawColor(game.rendPov, 0, 0, 0, 255);
	SDL_RenderClear(game.rendPov);
	SDL_SetRenderTarget(game.rendPov, NULL);


	set<pair<double, double>> cones;
	cones.insert({ player.rangle, player.langle });
	queue <pair <int, int>> q;
	q.push({ player.x, player.y });
	vector<vector<bool>> visited(game.sizey, vector<bool>(game.sizex));
	vector<pair<int, int>> offset = { {0, 0}, {game.width - 1 , 0}, {0, game.height - 1}, {game.width - 1, game.height - 1} };
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
			int xcor = game.width * j + offset[it].first, ycor = game.height * i + offset[it].second;
			double angle = ang(player.xpos, player.ypos, xcor, ycor);
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
		int pix[] = { -1 ,1 };
		for (auto const& range : cones) {
			double lv = range.second, rv = range.first;
			if (rv >= lcorner) break;
			if (lv < rcorner) continue;
			flag = 1;
			delq.push_back(range);

			if (rcorner > rv) {
				insq.push_back({ rv, rcorner });
				points[0] = { j * game.width + offset[rit].first, i * game.height + offset[rit].second };
			}
			else {
				pair<int, int> t = presek(rv, player, i, j, game);
				points[0] = { t.first,t.second };
			}

			if (lcorner < lv) {
				insq.push_back({ lcorner, lv }); 
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

		triangle(game, player.xpos, player.ypos, points[0].first, points[0].second, points[1].first, points[1].second, tekstura);
		trapistSir(game.rendPov,player, points[0].first, points[0].second, points[1].first, points[1].second);
		if (points[1] != points[2]) {
			triangle(game, player.xpos, player.ypos, points[1].first, points[1].second, points[2].first, points[2].second, tekstura);
			trapistSir(game.rendPov, player, points[1].first, points[1].second, points[2].first, points[2].second);
		}
	}

	SDL_Rect rect = { 0,0,200,200 };
	SDL_RenderCopy(game.rendPov, tekstura, NULL, &rect);
	SDL_DestroyTexture(tekstura);
}