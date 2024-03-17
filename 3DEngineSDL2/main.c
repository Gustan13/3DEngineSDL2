#include "commons.h"

int main(int argc, const char* argv[]) {
	SDL_Window* window = NULL;
	SDL_Surface* screen = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Event e;

	float fTheta = 0.0f;

	struct mesh* cube = loadMeshFromObj("ship.obj");
	if (cube == NULL) {
		printf("Could not load mesh\n");
		return -1;
	}
	struct mat4x4* m = createProjectionMatrix();

	struct vertex camera = {0,0,0};

	bool running = true;

	if (init(&window, &renderer, &screen) < 0)
		return -1;

	while (running) {
		struct mat4x4* matRotZ = NULL, * matRotX = NULL;
		matRotZ = createZRotationMatrix(fTheta);
		matRotX = createXRotationMatrix(fTheta);

		while (SDL_PollEvent(&e)) {

			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.sym == SDLK_ESCAPE)
					running = false;
			}
			else if (e.type == SDL_QUIT)
				running = false;
		}

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
		SDL_RenderClear(renderer);
		SDL_FillRect(screen->format, NULL, SDL_MapRGB(screen->format, 0x00, 0x00, 0x00));

		for (struct triangle* t = cube->firstTriangle; t != NULL; t = t->nextTriangle) {
			struct triangle triTransform;
			struct triangle *triRotatedZ, *triRotatedZX;
			
			triRotatedZ = rotateTriangle(t, Z, 3.14);
			triRotatedZX = rotateTriangle(triRotatedZ, Y, fTheta);

			triTransform = *triRotatedZX;
			triTransform.vertices[0].z = triRotatedZX->vertices[0].z + 8.0f;
			triTransform.vertices[1].z = triRotatedZX->vertices[1].z + 8.0f;
			triTransform.vertices[2].z = triRotatedZX->vertices[2].z + 8.0f;

			drawProjectedTriangle(renderer, &triTransform, camera, m);

			free(triRotatedZ);
			free(triRotatedZX);
		}

		fTheta += 0.0005f;
		SDL_RenderPresent(renderer);

		free(matRotX);
		free(matRotZ);
	}

	SDL_Quit();

	return 0;
}