#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int TILE_SIZE = 40;

void logSDLError(std::ostream &os, const std::string &msg) {
	os << msg << " Error: " << SDL_GetError() << '\n';
}

SDL_Texture* loadTexture(const std::string &filePath, SDL_Renderer *renderer) {
	SDL_Texture *texture = IMG_LoadTexture(renderer, filePath.c_str());
	if (texture == nullptr) {
		logSDLError(std::cout, "LoadTexture");
	}
	return texture;
}

void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y, int w, int h) {
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(renderer, texture, NULL, &dst);
}

void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y) {
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	renderTexture(texture, renderer, x, y, w, h);
}

void renderBackground(SDL_Texture *background, SDL_Renderer *renderer) {
	int yRepeat = SCREEN_HEIGHT / TILE_SIZE;
	int xRepeat = SCREEN_WIDTH / TILE_SIZE;
	for (int i = 0; i <= yRepeat; ++i) {
		for (int j = 0; j <= xRepeat; ++j) {
			renderTexture(background, renderer, TILE_SIZE * j, TILE_SIZE * i, TILE_SIZE, TILE_SIZE);
		}
	}
}

int main(int, char**) {
	std::cout << "Initializing SDL sub-systems...\n";
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		logSDLError(std::cout, "Init");
		return 1;
	}
	std::cout << "SDL sub-systems initialized!\n";
	std::cout << "Initializing image loading sub-system...\n";
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		logSDLError(std::cout, "IMG_Init");
		SDL_Quit();
		return 1;
	}
	std::cout << "Image loading sub-systems initialized!\n";
	std::cout << "Creating window...\n";
	SDL_Window *window = SDL_CreateWindow("Lesson 3", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		logSDLError(std::cout, "CreateWindow");
		SDL_Quit();
		return 1;
	}
	std::cout << "Window created!\n";
	std::cout << "Creating renderer...\n";
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (renderer == nullptr) {
		logSDLError(std::cout, "CreateRenderer");
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	std::cout << "Renderer created!\n";
	std::cout << "Loading background and image PNGs as textures...\n";
	SDL_Texture *background = loadTexture("background.png", renderer);
	SDL_Texture *image = loadTexture("image.png", renderer);
	if (background == nullptr || image == nullptr) {
		SDL_DestroyTexture(image);
		SDL_DestroyTexture(background);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}
	std::cout << "PNGs loaded and textures created!\n";
	std::cout << "Rendering...\n";
	// main loop
	for (int i = 0; i < 3; ++i) {
		SDL_RenderClear(renderer);

		renderBackground(background, renderer);

		int iW, iH;
		SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
		int x = SCREEN_WIDTH / 2 - iW / 2;
		int y = SCREEN_HEIGHT / 2 - iH / 2;
		renderTexture(image, renderer, x, y);

		SDL_RenderPresent(renderer);
		SDL_Delay(2000);
	}
	std::cout << "Rendering done!\n";
	std::cout << "Cleaning up and quitting...\n";
	SDL_DestroyTexture(image);
	SDL_DestroyTexture(background);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}