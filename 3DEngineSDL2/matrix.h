#pragma once

#ifndef _MATRIX
#define _MATRIX

void MultiplyMatrixVector(
	struct vertex* i,
	struct vertex* o,
	struct mat4x4* m
);

struct mat4x4* createZRotationMatrix(float fTheta);

struct mat4x4* createXRotationMatrix(float fTheta);

struct mat4x4* createProjectionMatrix();

#endif // !_MATRIX
