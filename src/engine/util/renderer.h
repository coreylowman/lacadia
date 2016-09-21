#ifndef RENDERER_H
#define RENDERER_H

#include "engine/util/shader.h"
#include "engine/util/object_model.h"
#include "engine/util/array_list.h"
#include "engine/util/rect.h"
#include "engine/util/mat4.h"
#include "engine/util/line.h"
#include "engine/util/obb.h"
#include "engine/util/terrain.h"

#define MAX_ASSETS 32
#define MAX_UI_RECTS 128
#define MAX_LINES 2048
#define MAX_TERRAINS 4
#define MAX_CHARS 64
#define MAX_CHAR_VERTS (6 * MAX_CHARS)

typedef struct CharacterStats {
  unsigned int textureID;
  int size[2];
  int bearing[2];
  unsigned int advance;
} CharacterStats;

typedef struct TextCharacterVertex {
  float x, y, u, v;
  Vec3 color;
} TextCharacterVertex;

typedef struct Renderer {
  // models and stuff aka assets
  Shader model_shader;
  int num_models;
  char *model_names[MAX_ASSETS];
  ObjectModel *models[MAX_ASSETS];
  ArrayList_m4 *model_model_matrices[MAX_ASSETS]; // ArrayList<Mat4>[MAX_ASSETS]
  unsigned int model_vbo[2], model_vao;

  // ui rects and stuff
  Shader ui_shader;
  int num_ui_rects;
  UIRect rects[MAX_UI_RECTS];
  Vec3 rect_colors[4 * MAX_UI_RECTS];
  unsigned int ui_vbo[2], ui_vao;

  // lines and stuff
  Shader line_shader;
  int num_lines;
  Line lines[MAX_LINES];
  unsigned int line_vbo, line_vao;

  // text
  Shader text_shader;
  CharacterStats char_stats[128];
  int num_character_vertices[128];
  TextCharacterVertex characters[128][MAX_CHAR_VERTS];
  unsigned int text_vbo, text_vao;

  // terrains and stuff
  Shader terrain_shader;
  int num_terrains;
  Terrain terrains[MAX_TERRAINS];
  unsigned int terrain_vbo, terrain_vao;
} Renderer;

Renderer *renderer_new();
void renderer_free(Renderer *self);

int renderer_get_model_id(Renderer *self, const char *name);
Obb renderer_get_model_obb(Renderer *self, int model_id);

void renderer_render(Renderer *self, Mat4 projection_matrix, Mat4 view_matrix);

void renderer_render_model(Renderer *self, int model_id, Mat4 model_matrix);
void renderer_render_rect(Renderer *self, Rect2 rect, Vec3 color);
void renderer_render_line(Renderer *self, Line line);
void renderer_render_terrain(Renderer *self, Terrain terrain);
void renderer_render_sphere(Renderer *self, Vec3 position);
void renderer_render_text(Renderer *self, const char *buffer, int len,
                          Vec3 xyscale, Vec3 color);

#endif
