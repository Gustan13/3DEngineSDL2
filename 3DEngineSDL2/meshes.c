#include <SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "structs.h"
#include "triangles.h"
#include "meshes.h"

struct mesh* createMesh() {
	struct mesh* m = NULL;
	m = malloc(sizeof(struct mesh));

	if (m == NULL)
		return m;

	m->firstTriangle = NULL;
	m->lastTriangle = NULL;

	return m;
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