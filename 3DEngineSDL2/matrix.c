#include <SDL.h>
#include <stdlib.h>

#include "structs.h"
#include "matrix.h"

void MultiplyMatrixVector(
	struct vertex* i,
	struct vertex* o,
	struct mat4x4* m
) {
	o->x = i->x * m->m[0][0] + i->y * m->m[1][0] + i->z * m->m[2][0] + m->m[3][0];
	o->y = i->x * m->m[0][1] + i->y * m->m[1][1] + i->z * m->m[2][1] + m->m[3][1];
	o->z = i->x * m->m[0][2] + i->y * m->m[1][2] + i->z * m->m[2][2] + m->m[3][2];
	float w = i->x * m->m[0][3] + i->y * m->m[1][3] + i->z * m->m[2][3] + m->m[3][3];

	if (w != 0.0f) {
		o->x /= w; o->y /= w; o->z /= w;
	}
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