#ifndef TERRAIN_H
#define TERRAIN_H

#include "vec3.h"

typedef struct Terrain {
    int asset_vao;
    int asset_vbo[2];
    int num_vertices;
    float *vertices;
    float *normals;
} Terrain;

Terrain terrain_new(Vec3 start, Vec3 dimensions);
void terrain_free(Terrain self);

void terrain_render(Terrain self);

#endif
