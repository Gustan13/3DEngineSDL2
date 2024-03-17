#pragma once

#ifndef _STRUCTS
#define _STRUCTS

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

#endif