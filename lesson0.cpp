#include<iostream>
#include<SDL2/SDL.h>

int main(int, char**){
	if (SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return 1;
	}
	std::cout << "SDL initialized successfully!\n";
	SDL_Quit();
	return 0;
}