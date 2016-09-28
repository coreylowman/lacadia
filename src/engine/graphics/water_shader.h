#ifndef WATER_SHADER_H
#define WATER_SHADER_H

#include "engine/graphics/shader.h"
#include "engine/util/rect.h"
#include "engine/util/vec3.h"
#include "engine/asset_manager.h"

#define MAX_WATER_CHUNKS 32
#define MAX_WATER_VERTICES (MAX_WATER_CHUNKS * 6)

typedef struct Renderer Renderer;

typedef void (*WaterShaderRenderCallback)(Renderer *renderer);

typedef struct WaterVertex {
  Vec3 position;
  float u, v;
} WaterVertex;

typedef struct WaterShader {
  Shader base_shader;

  int num_water_vertices;
  WaterVertex water_vertices[MAX_WATER_VERTICES];
  unsigned int vbo, vao;

  unsigned int distortion_texture;
  unsigned int normal_texture;

  unsigned int reflection_frame_buffer;
  unsigned int reflection_texture;
  unsigned int reflection_depth_buffer;

  unsigned int refraction_frame_buffer;
  unsigned int refraction_texture;
  unsigned int refraction_depth_texture;

  int reflection_texture_location;
  int refraction_texture_location;
  int distortion_texture_location;
  int depth_texture_location;
  int normal_texture_location;
  int move_factor_location;
  int camera_position_location;

  Vec3 *camera_position;
} WaterShader;

WaterShader *water_shader_new(AssetManager *asset_manager);
void water_shader_free(WaterShader *self);
void water_shader_add_chunk(WaterShader *self, Vec3 center, float width,
                            float length);

void water_shader_render_reflection(WaterShader *self, Renderer *renderer,
                                    WaterShaderRenderCallback renderCallback);
void water_shader_render_refraction(WaterShader *self, Renderer *renderer,
                                    WaterShaderRenderCallback renderCallback);

#endif
