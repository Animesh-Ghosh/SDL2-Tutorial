#include<iostream>
#include<SDL2/SDL.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void logSDLError(std::ostream &os, const std::string &msg) {
	os << msg << " Error: " << SDL_GetError() << '\n';
}

SDL_Texture* loadTexture(const std::string &filePath, SDL_Renderer *renderer) {
	SDL_Texture *texture = nullptr;
	SDL_Surface *loadedImage = SDL_LoadBMP(filePath.c_str());
	if (loadedImage != nullptr) {
		texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
		SDL_FreeSurface(loadedImage);
		if (texture == nullptr) {
			logSDLError(std::cout, "CreateTextureFromSurface");
		}
	}
	else {
		logSDLError(std::cout, "LoadBMP");
	}
	return texture;
}

void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, int x, int y) {
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
	SDL_RenderCopy(renderer, texture, NULL, &dst);
}

void renderBackground(SDL_Texture *background, SDL_Renderer *renderer, int bgWidth, int bgHeight) {
	int yRepeat = SCREEN_HEIGHT / bgHeight;
	int xRepeat = SCREEN_WIDTH / bgWidth;
	for (int i = 0; i < yRepeat; ++i) {
		for (int j = 0; j < xRepeat; ++j) {
			renderTexture(background, renderer, bgWidth * j, bgHeight * i);
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
	std::cout << "Creating window...\n";
	SDL_Window *window = SDL_CreateWindow("Lesson 2", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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
	std::cout << "Loading background and image BMPs as textures...\n";
	SDL_Texture *background = loadTexture("background.bmp", renderer);
	SDL_Texture *image = loadTexture("image.bmp", renderer);
	if (background == nullptr || image == nullptr) {
		SDL_DestroyTexture(image);
		SDL_DestroyTexture(background);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	std::cout << "BMPs loaded and textures created!\n";
	std::cout << "Rendering...\n";
	// main loop
	for (int i = 0; i < 3; ++i) {
		SDL_RenderClear(renderer);
		int bW, bH;
		SDL_QueryTexture(background, NULL, NULL, &bW, &bH);
		renderBackground(background, renderer,  bW, bH);

		int iW, iH;
		SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
		int x = SCREEN_WIDTH / 2 - iW / 2;
		int y = SCREEN_HEIGHT / 2 - iH / 2;
		renderTexture(image, renderer, x, y);

		SDL_RenderPresent(renderer);
		SDL_Delay(1000);
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