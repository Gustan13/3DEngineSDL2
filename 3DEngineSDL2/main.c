#include "commons.h"

struct vertex* createVertex(float x, float y, float z) {
	struct vertex* v = NULL;
	v = malloc(sizeof(struct triangle));
	
	if (v == NULL)
		return v;

	v->x = x;
	v->y = y;
	v->z = z;

	return v;
}

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

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Could not initialize SDL2: %s\n", SDL_GetError());
		return -1;
	}

	SDL_CreateWindowAndRenderer(640, 480, 0, &window, &renderer);
	if (window == NULL || renderer == NULL) {
		printf("Could not create WINDOW or RENDERER: %s\n", SDL_GetError());
		return -1;
	}

	screen = SDL_GetWindowSurface(window);
	if (screen == NULL) {
		printf("Could not create surface from window: %s\n", SDL_GetError());
		return -1;
	}

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
			struct triangle triProj = { 0, 0, 0 }, triTransform, triRotatedZ, triRotatedZX;

			MultiplyMatrixVector(&t->vertices[0], &triRotatedZ.vertices[0], matRotZ);
			MultiplyMatrixVector(&t->vertices[1], &triRotatedZ.vertices[1], matRotZ);
			MultiplyMatrixVector(&t->vertices[2], &triRotatedZ.vertices[2], matRotZ);

			MultiplyMatrixVector(&triRotatedZ.vertices[0], &triRotatedZX.vertices[0], matRotX);
			MultiplyMatrixVector(&triRotatedZ.vertices[1], &triRotatedZX.vertices[1], matRotX);
			MultiplyMatrixVector(&triRotatedZ.vertices[2], &triRotatedZX.vertices[2], matRotX);

			triTransform = triRotatedZX;
			triTransform.vertices[0].z = triRotatedZX.vertices[0].z + 8.0f;
			triTransform.vertices[1].z = triRotatedZX.vertices[1].z + 8.0f;
			triTransform.vertices[2].z = triRotatedZX.vertices[2].z + 8.0f;

			struct vertex normal, line1, line2;

			line1.x = triTransform.vertices[1].x - triTransform.vertices[0].x;
			line1.y = triTransform.vertices[1].y - triTransform.vertices[0].y;
			line1.z = triTransform.vertices[1].z - triTransform.vertices[0].z;

			line2.x = triTransform.vertices[2].x - triTransform.vertices[0].x;
			line2.y = triTransform.vertices[2].y - triTransform.vertices[0].y;
			line2.z = triTransform.vertices[2].z - triTransform.vertices[0].z;

			normal.x = line1.y * line2.z - line1.z * line2.y;
			normal.y = line1.z * line2.x - line1.x * line2.z;
			normal.z = line1.x * line2.y - line1.y * line2.x;

			float l = sqrtf(powf(normal.x, 2) + powf(normal.y, 2) + powf(normal.z, 2));

			normal.x /= l; normal.y /= l; normal.z /= l;

			if (normal.x * (triTransform.vertices[0].x - camera.x) +
				normal.y * (triTransform.vertices[0].y - camera.y) +
				normal.z * (triTransform.vertices[0].z - camera.z) < 0.0f) {

				MultiplyMatrixVector(&triTransform.vertices[0], &triProj.vertices[0], m);
				MultiplyMatrixVector(&triTransform.vertices[1], &triProj.vertices[1], m);
				MultiplyMatrixVector(&triTransform.vertices[2], &triProj.vertices[2], m);

				for (int i = 0; i < 3; i++) {
					triProj.vertices[i].x += 1.0f;
					triProj.vertices[i].y += 1.0f;

					triProj.vertices[i].x *= 0.5f * 640.0f;
					triProj.vertices[i].y *= 0.5f * 480.0f;
				}

				drawTriangle(triProj.vertices[0].x, triProj.vertices[0].y, triProj.vertices[1].x,
					triProj.vertices[1].y, triProj.vertices[2].x, triProj.vertices[2].y, renderer);
			}
		}

		fTheta += 0.001f;
		SDL_RenderPresent(renderer);

		free(matRotX);
		free(matRotZ);
	}

	SDL_Quit();

	return 0;
}