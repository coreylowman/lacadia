#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include "renderer.h"
#include "engine/util/string_helpers.h"

Vec3 light_position = {.data = {200, 100, 0}};

Renderer *renderer_new() {
  Renderer *self = malloc(sizeof(*self));

  self->num_models = 11;
  self->model_names[0] = "./assets/models/default_box";
  self->model_names[1] = "./assets/models/mage";
  self->model_names[2] = "./assets/models/hunter";
  self->model_names[3] = "./assets/models/bug";
  self->model_names[4] = "./assets/models/fireball";
  self->model_names[5] = "./assets/models/icicle";
  self->model_names[6] = "./assets/models/burn_particle";
  self->model_names[7] = "./assets/models/wall";
  self->model_names[8] = "./assets/models/sphere";
  self->model_names[9] = "./assets/models/frost_particle";
  self->model_names[10] = "./assets/models/coin";

  printf("Loading models... ");
  int i;
  for (i = 0; i < self->num_models; i++) {
    self->models[i] = obj_model_from_file(self->model_names[i]);
  }
  printf("done.\n");

  self->model_shader = model_shader_new(self->num_models, self->models);
  self->line_shader = line_shader_new();
  self->text_shader = text_shader_new();
  self->terrain_shader = terrain_shader_new();

  return self;
}

void renderer_free(Renderer *self) {
  model_shader_free(self->model_shader);
  line_shader_free(self->line_shader);
  text_shader_free(self->text_shader);
  terrain_shader_free(self->terrain_shader);

  int i;
  for (i = 0; i < self->num_models; i++) {
    obj_model_free(self->models[i]);
  }
  free(self);
}

int renderer_get_model_id(Renderer *self, const char *name) {
  int i;
  for (i = 0; i < self->num_models; i++) {
    if (string_equals(self->model_names[i], name))
      return i;
  }
  // default to default box
  return 0;
}

Obb renderer_get_model_obb(Renderer *self, int model_id) {
  return self->models[model_id]->bounding_box;
}

void renderer_render(Renderer *self, Camera camera) {
  shader_render((Shader *)self->model_shader, camera);
  shader_render((Shader *)self->line_shader, camera);
  shader_render((Shader *)self->text_shader, camera);
  shader_render((Shader *)self->terrain_shader, camera);

  shader_post_render((Shader *)self->model_shader);
  shader_post_render((Shader *)self->line_shader);
  shader_post_render((Shader *)self->text_shader);
  shader_post_render((Shader *)self->terrain_shader);
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
  int model_id = renderer_get_model_id(self, "./assets/sphere");
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
