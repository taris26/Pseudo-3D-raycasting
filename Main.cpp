#include <iostream>
#include <SDL.h>
#include <SDL_timer.h>
#include <vector>
#include <cmath>

using namespace std;

int sizex, sizey, width, height, tx, ty;
double langle = M_PI * 7 / 4 + 0.1, rangle = M_PI * 5 / 4 - 0.1;

void presek(double angle, int x, int y, vector<vector<bool>>& wall) {
	int i = y / height, j = x / width;
	int offx = 0, offy = 0;
	int sign[2] = { 1, -1 };
	while (!wall[i][j]) {
		offx += sign[cos(angle) < 0];
		offy = -offx * tan(angle);
		i = (y + offy) / height;
		j = (x + offx) / width;
	}
	tx = x + offx;
	ty = y + offy;
}

//void dfs(int x, int y, vector<vector<bool>>& wall, vector<vector<bool>>& visited) {
//	if (visited[x][y]) return;
//	if (wall[x][y]) {
//
//	}
//}



int main(int argc, char* argv[])
{
	// returns zero on success else non-zero
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("error initializing SDL: %s\n", SDL_GetError());
	}
	SDL_Window* win = SDL_CreateWindow("DOOM", // creates a window
									   SDL_WINDOWPOS_CENTERED,
									   SDL_WINDOWPOS_CENTERED,
									   1000, 1000, 0);
 
	// triggers the program that controls
	// your graphics hardware and sets flags
	Uint32 render_flags = SDL_RENDERER_ACCELERATED;
 
	// creates a renderer to render our images
	SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);

	int x = 4, y = 1;
 
	// controls animation loop
	int close = 0;
 
	// speed of box
	//int speed = 1500;

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
 
	// animation loop
	while (!close) {
		SDL_Event event;
 
		// Events management
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
 
			case SDL_QUIT:
				// handling of close button
				close = 1;
				break;
 
			case SDL_KEYDOWN:
				// keyboard API for key pressed
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
				//case SDL_SCANCODE_RIGHT:
				case SDL_SCANCODE_LEFT:
					langle += 0.1;
					rangle += 0.1;
				default:
					break;
				}
			}
		}

		// clears the screen
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

		presek(langle, width* x + width / 2, height* y + height / 2, wall);
		//cout << tx << ' ' << ty << ' ';
		SDL_RenderDrawLine(rend, width * x + width / 2, height * y + height / 2, tx, ty);
		presek(rangle, width * x + width / 2, height * y + height / 2, wall);
		//cout << tx << ' ' << ty << ' ';
		SDL_RenderDrawLine(rend, width * x + width / 2, height * y + height / 2, tx, ty);
 
		// triggers the double buffers
		// for multiple rendering
		SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
		SDL_RenderPresent(rend);
 
		// calculates to 60 fps
		SDL_Delay(1000 / 60);
	}
 
	// destroy texture
	//SDL_DestroyTexture(tex);
 
	// destroy renderer
	SDL_DestroyRenderer(rend);
 
	// destroy window
	SDL_DestroyWindow(win);
	 
	// close SDL
	SDL_Quit();
 
	return 0;
}