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

bool moveTexture(SDL_KeyboardEvent &event, int &x, int &y) {
	int increment = 1;
	bool quit = false;
	if (event.repeat) {
		std::cout << "Key press repeated!\n";
		increment = 10;
	}
	switch (event.keysym.sym) {
		case SDLK_UP:
			std::cout << "Up key pressed!\n";
			y -= increment;
			break;
		case SDLK_DOWN:
			std::cout << "Down key pressed!\n";
			y += increment;
			break;
		case SDLK_LEFT:
			std::cout << "Left key pressed!\n";
			x -= increment;
			break;
		case SDLK_RIGHT:
			std::cout << "Right key pressed!\n";
			x += increment;
			break;
		default:
			std::cout << "Some other key pressed!\n";
			quit = true;
	}
	return quit;
}

int main(int, char**) {
	std::cout << "Initializing SDL sub-systems...\n";
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		logSDLError(std::cout, "Init");
		return 1;
	}
	std::cout << "SDL sub-systems initialized!\n";
	std::cout << "Initializing image loading sub-systems...\n";
	if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		logSDLError(std::cout, "IMG_Init");
		SDL_Quit();
		return 1;
	}
	std::cout << "Image loading sub-systems initialized!\n";
	std::cout << "Creating window...\n";
	SDL_Window *window = SDL_CreateWindow("Lesson 4", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
	std::cout << "Loading PNG image as texture...\n";
	SDL_Texture *image = loadTexture("image-1.png", renderer);
	if (image == nullptr) {
		SDL_DestroyTexture(image);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		IMG_Quit();
		SDL_Quit();
		return 1;
	}
	std::cout << "PNG loaded and texture created!\n";
	std::cout << "Rendering...\n";
	// main loop
	SDL_Event event;
	bool quit = false;
	int x = 0, y = 0;
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					std::cout << "Window closed!\n";
					quit = true;
					break;
				case SDL_KEYDOWN:
					quit = moveTexture(event.key, x, y);
					break;
				case SDL_MOUSEBUTTONDOWN:
					std::cout << "Mouse clicked!\n";
					quit = true;
					break;
			}
		}
		SDL_RenderClear(renderer);
		renderTexture(image, renderer, x, y);
		SDL_RenderPresent(renderer);
	}
	std::cout << "Rendering done!\n";
	std::cout << "Cleaning up and quitting...\n";
	SDL_DestroyTexture(image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	IMG_Quit();
	SDL_Quit();
	return 0;
}