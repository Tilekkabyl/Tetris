#ifndef Window_hpp

#include <SDL.h>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <chrono>

using namespace std;

extern int map[20][10];
static int windowWidth = 10;
static int windowHeight = 20;
static int tile = 45;

struct bricks {
	int y[4];
	int x[4];
} extern typesOfTetrads[7];

struct tetrad : bricks {
	int my;
	int mx;
	int type;
} extern current;

class Window {
public:
	Window();
	~Window();

	void init(const char* title, bool fullscreen);

	void HandleEvent();
	void render();
	void clean();
	void update();
	void show();
	void Tetris();
	void setColor(int type);
	bool running();
	
private:

	bool isRunning;
	SDL_Window *window;
	SDL_Renderer *renderer;
	
};

#endif Window_hpp