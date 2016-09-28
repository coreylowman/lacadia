#ifndef TERRAIN_SHADER_H
#define TERRAIN_SHADER_H

#include "engine/graphics/shader.h"
#include "engine/util/terrain.h"

#define MAX_TERRAINS 8

typedef struct TerrainShader {
  Shader base_shader;

  int num_terrains;
  Terrain terrains[MAX_TERRAINS];
  unsigned int vbo, vao;
} TerrainShader;

TerrainShader *terrain_shader_new();
void terrain_shader_free(TerrainShader *self);

void terrain_shader_add_terrain(TerrainShader *self, Terrain terrain);

#endif
