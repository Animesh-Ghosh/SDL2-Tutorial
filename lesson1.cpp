#include<iostream>
#include<SDL2/SDL.h>

int main(int, char**) {
	std::cout << "Initializing SDL sub-systems...\n";
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		std::cout << "Init Error: " << SDL_GetError() << '\n';
		return 1;
	}
	std::cout << "SDL sub-systems initialized!\n";
	std::cout << "Creating window...\n";
	SDL_Window *window = SDL_CreateWindow("Hello, World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	if (window == nullptr) {
		std::cout << "CreateWindow Error: " << SDL_GetError() << '\n';
		SDL_Quit();
		return 1;
	}
	std::cout << "Window created!\n";
	std::cout << "Creating renderer...\n";
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
	if (renderer == nullptr) {
		std::cout << "CreateRenderer Error: " << SDL_GetError() << '\n';
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	std::cout << "Renderer created!\n";
	std::cout << "Loading BMP onto surface...\n";
	char imagePath[] = "hello.bmp";
	SDL_Surface *bmp = SDL_LoadBMP(imagePath);
	if (bmp == nullptr) {
		std::cout << "LoadBMP Error: " << SDL_GetError() << '\n';
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	std::cout << "BMP loaded onto surface!\n";
	std::cout << "Creating texture from surface and freeing said surface...\n";
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, bmp);
	SDL_FreeSurface(bmp);
	if (texture == nullptr) {
		std::cout << "CreateTextureFromSurface Error: " << SDL_GetError() << '\n';
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;
	}
	std::cout << "Texture created!\n";
	std::cout << "Rendering...\n";
	// main loop
	for (int i = 0; i < 3; ++i) {
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
		SDL_Delay(1000);
	}
	std::cout << "Rendering done!\n";
	std::cout << "Cleaning up and quitting...\n";
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}