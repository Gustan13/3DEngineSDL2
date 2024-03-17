#include <SDL.h>
#include "structs.h"
#include "triangles.h"
#include "draw.h"

void drawProjectedTriangle(SDL_Renderer* renderer, struct triangle* tri, struct vertex camera, struct mat4x4* m) {
		struct triangle triProj = { 0, 0, 0 }, triTransform;

		triTransform = *tri;

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