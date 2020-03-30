#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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

void renderTexture(SDL_Texture *texture, SDL_Renderer *renderer, SDL_Rect dst, SDL_Rect *clip = nullptr) {
	SDL_RenderCopy(renderer, texture, clip, &dst);
}

void renderTexture(SDL_Texture *texture,  SDL_Renderer *renderer, int x, int y, SDL_Rect *clip = nullptr) {
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr) {
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else {
		SDL_QueryTexture(texture, NULL, NULL, &dst.w, &dst.h);
	}
	renderTexture(texture, renderer, dst, clip);
}

bool clipTexture(SDL_KeyboardEvent &event, int &clip) {
	bool quit = false;
	switch (event.keysym.sym) {
		case SDLK_1:
			std::cout << "Showing clip 1!\n";
			clip = 0;
			break;
		case SDLK_2:
			std::cout << "Showing clip 2!\n";
			clip = 1;
			break;
		case SDLK_3:
			std::cout << "Showing clip 3!\n";
			clip = 2;
			break;
		case SDLK_4:
			std::cout << "Showing clip 4!\n";
			clip = 3;
			break;
		default:
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
	SDL_Texture *image = loadTexture("image-2.png", renderer);
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
	int iW = 100, iH = 100;
	int x = SCREEN_WIDTH / 2 - iW / 2;
	int y = SCREEN_HEIGHT / 2 - iH / 2;
	SDL_Rect clips[4];
	int idx = 0;
	for (int i = 0; i < 2; ++i) {
		for (int j = 0; j < 2; ++j) {
			clips[idx].x = iW * i;
			clips[idx].y = iH * j;
			clips[idx].w = iW;
			clips[idx].h = iH;
			++idx;
		}
	}
	int clip = 0;
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
					quit = clipTexture(event.key, clip);
					break;
				case SDL_MOUSEBUTTONDOWN:
					std::cout << "Mouse clicked!\n";
					quit = true;
					break;
			}
		}
		SDL_RenderClear(renderer);
		renderTexture(image, renderer, x, y, &clips[clip]);
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