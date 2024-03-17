#include <SDL.h>
#include "structs.h"
#include "triangles.h"

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

void drawTriangle(
	float x1, float y1,
	float x2, float y2,
	float x3, float y3,
	SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0xff);

	SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
	SDL_RenderDrawLineF(renderer, x2, y2, x3, y3);
	SDL_RenderDrawLineF(renderer, x3, y3, x1, y1);
}