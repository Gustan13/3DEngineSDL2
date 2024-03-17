#pragma once

#ifndef _DRAW
#define _DRAW

void drawProjectedTriangle(SDL_Renderer* renderer, struct mesh* mesh, struct vertex camera, struct mat4x4* m);

#endif