#include <SDL.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define PI 3.14159

struct vertex {
	float x, y, z;
};

struct triangle {
	struct vertex vertices[3];
	struct triangle* nextTriangle;
};

struct mesh {
	struct triangle* firstTriangle;
	struct triangle* lastTriangle;
};

struct mat4x4 {
	float m[4][4];
};

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

struct triangle* createTriangle(struct vertex v1, struct vertex v2, struct vertex v3) {
	struct triangle* t = NULL;
	t = malloc(sizeof(struct triangle));

	if (t == NULL)
		return t;

	t->vertices[0] = v1;
	t->vertices[1] = v2;
	t->vertices[2] = v3;

	t->nextTriangle = NULL;

	return t;
}

struct mesh* createMesh() {
	struct mesh* m = NULL;
	m = malloc(sizeof(struct mesh));

	if (m == NULL)
		return m;

	m->firstTriangle = NULL;
	m->lastTriangle = NULL;

	return m;
}

int addTriangleToMesh(struct triangle* tri, struct mesh* m) {

	struct triangle* triangle_ptr = NULL;
	triangle_ptr = m->firstTriangle;

	if (m == NULL || tri == NULL)
		return 1;

	if (triangle_ptr == NULL) {
		m->firstTriangle = tri;
		m->lastTriangle = tri;
		return 0;
	}

	while (triangle_ptr->nextTriangle != NULL)
		triangle_ptr = triangle_ptr->nextTriangle;

	triangle_ptr->nextTriangle = tri;
	m->lastTriangle = tri;

	return 0;
}

struct mat4x4* createProjectionMatrix() {
	float fNear = 0.1f;
	float fFar = 1000.0f;
	float fFov = 90.0f;
	float aspectRatio = 480.0f / 640.0f;
	float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

	struct mat4x4* mat = NULL;
	mat = malloc(sizeof(struct mat4x4));
	if (mat == NULL)
		return mat;

	SDL_memset(mat->m, 0, sizeof(float) * 16);

	mat->m[0][0] = aspectRatio * fFovRad;
	mat->m[1][1] = fFovRad;
	mat->m[2][2] = fFar / (fFar - fNear);
	mat->m[3][2] = (-fFar * fNear) / (fFar - fNear);
	mat->m[2][3] = 1.0f;
	mat->m[3][3] = 0.0f;

	return mat;
}

struct mat4x4* createXRotationMatrix(float fTheta) {
	struct mat4x4* mat = NULL;
	mat = malloc(sizeof(struct mat4x4));
	if (mat == NULL)
		return mat;

	SDL_memset(mat->m, 0, sizeof(float) * 16);

	mat->m[0][0] = 1;
	mat->m[1][1] = cosf(fTheta * 0.5f);
	mat->m[1][2] = sinf(fTheta * 0.5f);
	mat->m[2][1] = -sinf(fTheta * 0.5f);
	mat->m[2][2] = cosf(fTheta * 0.5f);
	mat->m[3][3] = 1;

	return mat;
}

struct mat4x4* createZRotationMatrix(float fTheta) {
	struct mat4x4* mat = NULL;
	mat = malloc(sizeof(struct mat4x4));
	if (mat == NULL)
		return mat;

	SDL_memset(mat->m, 0, sizeof(float) * 16);

	mat->m[0][0] = cosf(fTheta);
	mat->m[0][1] = sinf(fTheta);
	mat->m[1][0] = -sinf(fTheta);
	mat->m[1][1] = cosf(fTheta);
	mat->m[2][2] = 1;
	mat->m[3][3] = 1;

	return mat;
}

void MultiplyMatrixVector(
	struct vertex* i,
	struct vertex* o,
	struct mat4x4* m
) {
	o->x = i->x * m->m[0][0] + i->y * m->m[1][0] + i->z * m->m[2][0] + m->m[3][0];
	o->y = i->x * m->m[0][1] + i->y * m->m[1][1] + i->z * m->m[2][1] + m->m[3][1];
	o->z = i->x * m->m[0][2] + i->y * m->m[1][2] + i->z * m->m[2][2] + m->m[3][2];
	float w = i->x* m->m[0][3] + i->y * m->m[1][3] + i->z * m->m[2][3] + m->m[3][3];

	if (w != 0.0f) {
		o->x /= w; o->y /= w; o->z /= w;
	}
}

struct mesh* loadMeshFromObj(const char* filename) {
	FILE* f = NULL;
	struct mesh* mesh = NULL;

	fopen_s(&f, filename, "r");
	if (f == NULL)
		return NULL;

	int fSize = 0;
	fseek(f, 0, SEEK_END);
	fSize = ftell(f);
	rewind(f);

	struct vertex* v = malloc(sizeof(struct vertex) * fSize);
	int i = 0;

	mesh = createMesh();

	char* buffer = malloc(sizeof(char) * 1024);
	if (buffer == NULL)
		return NULL;

	while (fgets(buffer, sizeof(char) * 1023, f)) {
		char* nextToken = NULL;
		char* tok = strtok_s(buffer, " ", &nextToken);

		struct triangle* t = NULL;
		int v1, v2, v3;

		switch (tok[0])
		{
		case '#':
			continue;
		case 'v':
			tok = strtok_s(NULL, " ", &nextToken);
			v[i].x = (float)atof(tok);
			tok = strtok_s(NULL, " ", &nextToken);
			v[i].y = (float)atof(tok);
			tok = strtok_s(NULL, " ", &nextToken);
			v[i].z = (float)atof(tok);
			tok = strtok_s(NULL, " ", &nextToken);
			i++;
			break;
		case 'f':
			tok = strtok_s(NULL, " ", &nextToken);
			v1 = atoi(tok);
			tok = strtok_s(NULL, " ", &nextToken);
			v2 = atoi(tok);
			tok = strtok_s(NULL, " ", &nextToken);
			v3 = atoi(tok);
			tok = strtok_s(NULL, " ", &nextToken);

			t = createTriangle(v[v1 - 1], v[v2 - 1], v[v3 - 1]);
			addTriangleToMesh(t, mesh);
			break;
		default:
			break;
		}
	}

	return mesh;
}

void drawTriangle(
	float x1, float y1,
	float x2, float y2,
	float x3, float y3,
	SDL_Renderer* renderer)
{
	SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);

	SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
	SDL_RenderDrawLineF(renderer, x2, y2, x3, y3);
	SDL_RenderDrawLineF(renderer, x3, y3, x1, y1);
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