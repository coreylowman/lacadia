#ifndef RENDERER_H
#define RENDERER_H

#include "engine/util/camera.h"
#include "engine/util/object_model.h"
#include "engine/graphics/model_shader.h"
#include "engine/graphics/line_shader.h"
#include "engine/graphics/text_shader.h"
#include "engine/graphics/terrain_shader.h"
#include "engine/graphics/texture_shader.h"
#include "engine/graphics/water_shader.h"
#include "engine/asset_manager.h"

typedef struct Renderer {
  //reference to asset manager, we don't own
  AssetManager *asset_manager;

  ModelShader *model_shader;
  LineShader *line_shader;
  TextShader *text_shader;
  TerrainShader *terrain_shader;
  TextureShader *texture_shader;
  WaterShader *water_shader;
} Renderer;

Renderer *renderer_new(AssetManager *asset_manager);
void renderer_free(Renderer *self);

int renderer_get_model_id(Renderer *self, const char *name);
Obb renderer_get_model_obb(Renderer *self, int model_id);

void renderer_render(Renderer *self, Camera camera);

void renderer_render_model(Renderer *self, int model_id, Mat4 model_matrix);
// void renderer_render_rect(Renderer *self, Rect2 rect, Vec3 color);
void renderer_render_line(Renderer *self, Vec3 start, Vec3 end);
void renderer_render_terrain(Renderer *self, Terrain terrain);
void renderer_render_sphere(Renderer *self, Vec3 position);
void renderer_render_text(Renderer *self, const char *buffer, int len,
                          Vec3 xyscale, Vec3 color);
void renderer_render_texture(Renderer *self, Vec3 center, Vec3 left_offset, Vec3 top_offset, int texture_id);
void renderer_render_water(Renderer *self, Vec3 center, float width, float length);

#endif
