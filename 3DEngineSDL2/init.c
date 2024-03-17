#include <SDL.h>
#include "init.h"

int init(SDL_Window** w, SDL_Renderer** r, SDL_Surface** s) {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Could not initialize SDL2: %s\n", SDL_GetError());
		return -1;
	}

	*w = SDL_CreateWindow("3D Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
	if (*w == NULL) {
		printf("Could not create window: %s\n", SDL_GetError());
		return -1;
	}

	*r = SDL_CreateRenderer(*w, 0, SDL_RENDERER_ACCELERATED);
	if (*r == NULL) {
		printf("Could not create renderer: %s\n", SDL_GetError());
		return -1;
	}

	*s = SDL_GetWindowSurface(*w);
	if (*s == NULL) {
		printf("Could not create surface from window: %s\n", SDL_GetError());
		return -1;
	}

	return 0;
}