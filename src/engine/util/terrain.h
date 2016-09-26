#ifndef TERRAIN_H
#define TERRAIN_H

#include "vec3.h"

typedef struct TerrainVertex {
  float position[3];
  float normal[3];
  float color[3];
} TerrainVertex;

static TerrainVertex TERRAIN_VERTEX_DEFAULT = {
    .position = {0, 0, 0}, .normal = {0, 0, 0}, .color = {0, 0}};

typedef void (*TerrainVertexCallback)(TerrainVertex *vertex);

typedef struct Terrain {
  int unit_size;
  int width;
  int length;
  int height;

  float *height_map;

  int num_vertices;
  TerrainVertex *vertices;
} Terrain;

Terrain terrain_new(TerrainVertexCallback vertexCallback, int width, int height,
                    int length, int unit_size);
void terrain_regen(Terrain *t, TerrainVertexCallback vertexCallback);
void terrain_free(Terrain self);

float terrain_get_height(Terrain *self, float x, float y);

#endif
