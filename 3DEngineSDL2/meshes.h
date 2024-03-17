#pragma once

#ifndef _MESHES
#define _MESHES

struct mesh* createMesh();

struct mesh* loadMeshFromObj(const char* filename);

#endif