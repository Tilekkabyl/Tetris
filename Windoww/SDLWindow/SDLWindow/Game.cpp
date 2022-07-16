#include "Game.h"

Window::Window()
{}
Window::~Window()
{}

int map[20][10], i;
bool currentTetrad, flag;
auto start = chrono::steady_clock::now();
float delay = 0.2;
bricks typesOfTetrads[7] = {
	/*I*/	{{-1, -1, -1, -1}, {-2, -1, 0, 1}},
	/*Z*/	{{-1, -1, 0, 0}, {-2, -1, -1, 0}},
	/*S*/	{{-1, -1, 0, 0}, {0, -1, -1, -2}},
	/*Q*/	{{-1, -1, 0, 0}, {-1, 0, -1, 0}},
	/*T*/	{{-1, 0, 0, 0}, {-1, -2, -1, 0}},
	/*L*/	{{-1, 0, 0, 0}, {-2, -2, -1, 0}},
	/*J*/	{{-1, 0, 0, 0}, {0, -2, -1, 0}}};
tetrad current;

void Window::init(const char* title, bool fullscreen) {
	
	srand(time(0));
	memset(map, -1, sizeof(map[0][0]) * 20 * 10);

	int flags = 0;
	if (fullscreen)
		flags = SDL_WINDOW_FULLSCREEN;
	

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		cout << "Initialised!" << endl;

		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth * tile, windowHeight * tile, flags);
		if(window)
			cout << "Window created!" << endl;

		renderer = SDL_CreateRenderer(window, -1, false);
		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			cout << "Renderer created!" << endl;
		}

		isRunning = true;
	} 
	else {
		isRunning = false;
		return;
	}

}

bool checkBorders(int go);

tetrad rotate(tetrad given);

bool check(tetrad temporary);

void Window::setColor(int type) {
	switch (type) {
	case 0:
		SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
		break;
	case 1:
		SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
		break;
	case 2:
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
		break;
	case 3:
		SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
		break;
	case 4:
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		break;
	case 5:
		SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
		break;
	case 6:
		SDL_SetRenderDrawColor(renderer, 127, 0, 255, 255);
		break;
	}
}

void Window::HandleEvent() {
	SDL_Event event;
	const Uint8* keyPressed = SDL_GetKeyboardState(NULL);
	if (SDL_PollEvent(&event)) {
		if (flag) {
			flag = false;
			return;
		}
		if (event.type == SDL_QUIT)
			isRunning = false;
		if (keyPressed[SDL_SCANCODE_A]) {
			if (checkBorders(-1))
				current.mx--;
			cout << "A";
		}
		if (keyPressed[SDL_SCANCODE_D]) {
			if (checkBorders(1))
				current.mx++;
			cout << "D";
		}

		if (keyPressed[SDL_SCANCODE_W]) {
			if (check(rotate(current)))
				current = rotate(current);
		}

		flag = true;
	}
	//cout << endl;
}

void Window::render() {
	i = 0;

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	for (int i = 0; i <= windowWidth * tile; i += tile)
		SDL_RenderDrawLine(renderer, i, 0, i, windowHeight * tile);
	for (int i = 0; i <= windowHeight * tile; i += tile)
		SDL_RenderDrawLine(renderer, 0, i, windowWidth * tile, i);
	
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++) {
			if (map[i][j] != -1) {
				setColor(map[i][j]);
				SDL_Rect rect = { j * tile + 1, i * tile + 1, tile - 1, tile - 1 };
				SDL_RenderFillRect(renderer, &rect);
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		setColor(current.type);
		SDL_Rect rect = { (current.x[i] + current.mx) * tile + 1, (current.y[i] + current.my) * tile + 1, tile - 1, tile - 1};
		SDL_RenderFillRect(renderer, &rect);
	}

	SDL_RenderPresent(renderer);
	//SDL_Delay(10);
}

void Window::update() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
}

void Window::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	cout << "Window cleared";
}

bool check(tetrad temporary) {
	for (int i = 0; i < 4; i++) {
		int y = current.y[i] + current.my, x = current.x[i] + current.mx;
		if (map[y][x] != -1 || x < 0 || x > 9 || y < 0 || y > 19)
			return false;
	}
	return true;
}

bool checkBottom() {
	for (int i = 0; i < 4; i++) {
		int y = current.y[i] + current.my, x = current.x[i] + current.mx;
		if (y + 1 < 0 || map[y + 1][x] != -1) {
			return false;
		}
	}
	return true;
}

bool checkBorders(int go) {
	for (int i = 0; i < 4; i++) {
		int y = current.y[i] + current.my, x = current.x[i] + current.mx;
		if (x + go > 9 || x + go < 0 || map[y][x + go] != -1)
			return false;
	}

	//cout << "-----------------------------------------------------" << endl;
	return true;
}

bool checkTime() {
	auto end = std::chrono::steady_clock::now();
	chrono::duration<double> elapsed_seconds = end - start;
	//std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";
	if (elapsed_seconds.count() > delay) {
		start = std::chrono::steady_clock::now();
		return true;
	}
	return false;
}

void shiftEverythingFrom(int y) {
	for (int i = 0; i < 10; i++) {
		map[y][i] = -1;
	}
	for (int i = y; i > 0; i--) {
		for (int j = 0; j < 10; j++) {
			map[i][j] = map[i - 1][j];
		}
	}
}

void checkLine() {
	for (int i = 0; i < 20; i++) {
		bool temporary = true;
		for (int j = 0; j < 10 && temporary; j++) {
			if (map[i][j] == -1) {
				temporary = false;
			}
		}
		if (temporary) {
			shiftEverythingFrom(i);
		}
	}
}

tetrad rotate(tetrad given) {
	tetrad tempo = given;
	for (int i = 0; i < 4; i++) {
		int x = -1 * (tempo.y[i] + 1);
		tempo.y[i] = tempo.x[i];
		tempo.x[i] = x;
	}
	return tempo;
}

tetrad create(int type) {
	tetrad temporary;
	for (int i = 0; i < 4; i++) {
		temporary.x[i] = typesOfTetrads[type].x[i];
		temporary.y[i] = typesOfTetrads[type].y[i];
	}
	temporary.mx = 5;
	temporary.my = 1;
	temporary.type = type;
	currentTetrad = true;
	return temporary;
}

void animationDown() {
	if (checkBottom()) {
		current.my++;
	}
	else {
		for (int i = 0; i < 4; i++) {
			map[current.y[i] + current.my][current.x[i] + current.mx] = current.type;
		}
		currentTetrad = false;
	}
}

void Window::Tetris() {
	

	if(!currentTetrad)
		isRunning = check(current = create(rand() % 7));
	else {

		if (checkTime()) {
			animationDown();
		}
		checkLine();
	}
}

bool Window::running() {
	return isRunning;
}