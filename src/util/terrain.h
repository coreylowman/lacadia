#ifndef TERRAIN_H
#define TERRAIN_H

#include "vec3.h"

typedef struct Terrain {
    int num_floats;
    float *vertices;
    float *normals;
} Terrain;

Terrain terrain_new();
void terrain_regen(Terrain *t);
void terrain_free(Terrain self);

#endif
