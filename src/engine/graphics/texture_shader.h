#ifndef TEXTURE_SHADER_H
#define TEXTURE_SHADER_H

#include "engine/util/vec3.h"
#include "engine/graphics/shader.h"
#include "engine/asset_manager.h"

#define MAX_TEXTURES 32
#define MAX_TEXTURE_VERTICES 8

typedef struct TextureVertex {
    Vec3 position;
    float u, v;
} TextureVertex;

typedef struct TextureShader {
  Shader base_shader;

  AssetManager *asset_manager;

  int num_textures;
  int num_texture_vertices[MAX_TEXTURES];
  int texture_ids[MAX_TEXTURES];
  TextureVertex texture_vertices[MAX_TEXTURES][MAX_TEXTURE_VERTICES];
  unsigned int vbo, vao;

  int projection_matrix_location;
  int view_matrix_location;
  int texture_location;
} TextureShader;

TextureShader *texture_shader_new(AssetManager *asset_manager);
void texture_shader_free(TextureShader *self);
void texture_shader_add_texture(TextureShader *self, Vec3 center, Vec3 left_offset, Vec3 top_offset, int texture_id);

#endif
