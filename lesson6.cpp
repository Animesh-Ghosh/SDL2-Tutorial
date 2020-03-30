#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void logSDLError(std::ostream &os, const std::string &msg) {
	os << msg << " Error: " << SDL_GetError() << '\n';
}

SDL_Texture* renderText(const std::string &message, const std::string filePath, SDL_Colour colour, int fontSize, SDL_Renderer *renderer) {
	TTF_Font *font = TTF_OpenFont(filePath.c_str(), fontSize);
	if (font == nullptr) {
		logSDLError(std::cout, "OpenFont");
		return nullptr;
	}
	SDL_Surface *surface = TTF_RenderText_Blended(font, message.c_str(), colour);
	if (surface == nullptr) {
		logSDLError(std::cout, "RenderText_Blended");
		TTF_CloseFont(font);
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
	if (texture == nullptr) {
		logSDLError(std::cout, "CreateTextureFromSurface");
	}
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
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

int main(int, char**) {
	std::cout << "Initializing SDL sub-systems...\n";
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		logSDLError(std::cout, "Init");
		return 1;
	}
	std::cout << "SDL sub-systems initialized!\n";
	std::cout << "Initializing font loading sub-systems...\n";
	if (TTF_Init() != 0) {
		logSDLError(std::cout, "TTF_Init");
		SDL_Quit();
		return 1;
	}
	std::cout << "Font loading sub-systems initialized!\n";
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
	std::cout << "Loading TTF font as texture...\n";
	SDL_Colour colour = { 255, 255, 255, 255 };
	SDL_Texture *image = renderText("TTF fonts are kewl!", "sample.ttf", colour, 64, renderer);
	if (image == nullptr) {
		SDL_DestroyTexture(image);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}
	std::cout << "Font loaded and texture created!\n";
	std::cout << "Rendering...\n";
	int iW, iH;
	SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;
	SDL_Event event;
	bool quit = false;
	// main loop
	while (!quit) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					std::cout << "Window closed!\n";
					quit = true;
					break;
				case SDL_KEYDOWN:
					std::cout << "Key pressed!\n";
					quit = true;
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
	TTF_Quit();
	SDL_Quit();
	return 0;
}