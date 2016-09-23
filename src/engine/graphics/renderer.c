#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include "renderer.h"
#include "engine/util/string_helpers.h"

Vec3 light_position = {.data = {200, 100, 0}};

Renderer *renderer_new(AssetManager *asset_manager) {
  Renderer *self = malloc(sizeof(*self));

  self->asset_manager = asset_manager;

  self->model_shader = model_shader_new(self->asset_manager);
  self->texture_shader = texture_shader_new(self->asset_manager);
  self->line_shader = line_shader_new();
  self->text_shader = text_shader_new();
  self->terrain_shader = terrain_shader_new();
  self->water_shader = water_shader_new();

  return self;
}

void renderer_free(Renderer *self) {
  self->asset_manager = NULL;

  model_shader_free(self->model_shader);
  texture_shader_free(self->texture_shader);
  line_shader_free(self->line_shader);
  text_shader_free(self->text_shader);
  terrain_shader_free(self->terrain_shader);
  water_shader_free(self->water_shader);

  free(self);
}

int renderer_get_model_id(Renderer *self, const char *name) {
  return asset_manager_get_model_id(self->asset_manager, name);
}

Obb renderer_get_model_obb(Renderer *self, int model_id) {
  return asset_manager_get_model(self->asset_manager, model_id)->bounding_box;
}

static void render(Renderer *self, Camera camera) {
	shader_render((Shader *)self->terrain_shader, camera);
	shader_render((Shader *)self->model_shader, camera);
	shader_render((Shader *)self->texture_shader, camera);
	shader_render((Shader *)self->line_shader, camera);
}

static void post_render(Renderer *self) {
  shader_post_render((Shader *)self->terrain_shader);
  shader_post_render((Shader *)self->model_shader);
  shader_post_render((Shader *)self->texture_shader);
  shader_post_render((Shader *)self->line_shader);
}

unsigned char image[100];

void renderer_render(Renderer *self, Camera camera) {
  water_shader_pre_reflection_render(self->water_shader, camera);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  render(self, camera);
  water_shader_post_reflection_render(self->water_shader, camera);

  renderer_render_texture(self, (Vec3) { .x = -30, .y = 30, .z = 0 }, (Vec3) { .x = -15, 0, 0 }, (Vec3) { .x = 0, .y = 0, .z = -15 }, self->water_shader->reflection_texture);

  render(self, camera);
  shader_render((Shader *)self->water_shader, camera);
  shader_render((Shader *)self->text_shader, camera);

  post_render(self);
  shader_post_render((Shader *)self->water_shader);
  shader_post_render((Shader *)self->text_shader);
}

void renderer_render_model(Renderer *self, int model_id, Mat4 model_matrix) {
  model_shader_add_model(self->model_shader, model_id, model_matrix);
}

void renderer_render_line(Renderer *self, Line line) {
  line_shader_add_line(self->line_shader, line);
}

void renderer_render_terrain(Renderer *self, Terrain terrain) {
  terrain_shader_add_terrain(self->terrain_shader, terrain);
}

void renderer_render_sphere(Renderer *self, Vec3 position) {
  int model_id = renderer_get_model_id(self, "sphere");
  Mat4 model_matrix;
  mat4_ident(&model_matrix);
  mat4_translate(&model_matrix, position);
  mat4_transpose(&model_matrix);
  renderer_render_model(self, model_id, model_matrix);
}

void renderer_render_text(Renderer *self, const char *buffer, int len,
                          Vec3 xyscale, Vec3 color) {
  text_shader_add_text(self->text_shader, buffer, len, xyscale, color);
}

void renderer_render_texture(Renderer *self, Vec3 center, Vec3 left_offset, Vec3 top_offset, int texture_id) {
  texture_shader_add_texture(self->texture_shader, center, left_offset, top_offset, texture_id);
}

void renderer_render_water(Renderer *self, Vec3 center, float width, float length) {
  water_shader_add_chunk(self->water_shader, center, width, length);
}
