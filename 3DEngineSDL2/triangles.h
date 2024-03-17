#pragma once

#ifndef _TRIANGLES
#define _TRIANGLES

struct triangle* createTriangle(struct vertex v1, struct vertex v2, struct vertex v3);

int addTriangleToMesh(struct triangle* tri, struct mesh* m);

void drawTriangle(
	float x1, float y1,
	float x2, float y2,
	float x3, float y3,
	SDL_Renderer* renderer);

#endif