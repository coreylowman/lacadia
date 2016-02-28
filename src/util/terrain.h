#ifndef TERRAIN_H
#define TERRAIN_H

#include "vec3.h"

typedef struct TerrainVertex {
    float position[3];
    float normal[3];
    float texture[2];
} TerrainVertex;

typedef struct Terrain {
    // int num_floats;
    // float *vertices;
    // float *normals;

    // int num_texture_floats;
    // float *texture_coords;

    // int num_texture_inds;
    // int *texture_inds;

    int num_grass_vertices;
    TerrainVertex *grass_vertices;

    int num_grass_dirt_vertices;
    TerrainVertex *grass_dirt_vertices;

    int num_dirt_vertices;
    TerrainVertex *dirt_vertices;
} Terrain;

Terrain terrain_new();
void terrain_regen(Terrain *t);
void terrain_free(Terrain self);

#endif
