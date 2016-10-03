#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "renderer.h"
#include "engine/util/camera.h"
#include "texture_shader.h"

static void pre_render(Shader *self, Renderer *renderer);
static void render(Shader *self);
static void post_render(Shader *self);

TextureShader *texture_shader_new(AssetManager *asset_manager) {
  TextureShader *self = malloc(sizeof(*self));
  int result = shader_init(
      &self->base_shader, "./assets/shaders/texture_vert.glsl",
      "./assets/shaders/texture_frag.glsl", pre_render, render, post_render);
  if (result) {
    printf("Error loading texture shader\n");
    exit(result);
  }

  self->asset_manager = asset_manager;

  int i;
  for (i = 0; i < MAX_TEXTURES; i++) {
    self->num_texture_vertices[i] = 0;
    self->texture_ids[i] = 0;
  }
  self->num_textures = 0;
  glGenVertexArrays(1, &self->vao);
  glBindVertexArray(self->vao);

  glGenBuffers(1, &self->vbo);

  glBindBuffer(GL_ARRAY_BUFFER, self->vbo);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TextureVertex),
                        (const GLvoid *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(TextureVertex),
                        (const GLvoid *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  self->texture_location =
      glGetUniformLocation(self->base_shader.program, "tex");
  return self;
}

void texture_shader_free(TextureShader *self) { free(self); }

static void pre_render(Shader *shader, Renderer *renderer) {
  TextureShader *self = (TextureShader *)shader;
  glUseProgram(shader->program);
  glUniformMatrix4fv(shader->projection_matrix_location, 1, GL_TRUE,
                     &renderer->projection_matrix.data[0]);
  glUniformMatrix4fv(shader->view_matrix_location, 1, GL_TRUE,
                     &renderer->view_matrix.data[0]);
  glUniform1i(self->texture_location, 0);
  glActiveTexture(GL_TEXTURE0);
}

static void render(Shader *shader) {
  TextureShader *self = (TextureShader *)shader;
  int i;
  for (i = 0; i < self->num_textures; i++) {
    if (self->num_texture_vertices[i] == 0)
      continue;

    glBindVertexArray(self->vao);

    glBindTexture(GL_TEXTURE_2D, self->texture_ids[i]);

    glBindBuffer(GL_ARRAY_BUFFER, self->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 self->num_texture_vertices[i] * sizeof(TextureVertex),
                 &self->texture_vertices[i][0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArraysInstanced(GL_TRIANGLES, 0, self->num_texture_vertices[i],
                          self->num_texture_vertices[i] / 6);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
  }
}

static void post_render(Shader *shader) {
  TextureShader *self = (TextureShader *)shader;
  int i;
  for (i = 0; i < MAX_TEXTURES; i++) {
    self->num_texture_vertices[i] = 0;
  }
  self->num_textures = 0;
}

void texture_shader_add_texture(TextureShader *self, Vec3 center,
                                Vec3 left_offset, Vec3 top_offset,
                                int texture_id) {
  if (self->num_textures >= MAX_TEXTURES) {
    printf("error: too many textures\n");
    return;
  }

  Vec3 bottom_offset = vec3_scale(top_offset, -1);
  Vec3 right_offset = vec3_scale(left_offset, -1);

  Vec3 top_left = vec3_add(vec3_add(center, top_offset), left_offset);
  Vec3 top_right = vec3_add(vec3_add(center, top_offset), right_offset);
  Vec3 bottom_right = vec3_add(vec3_add(center, bottom_offset), right_offset);
  Vec3 bottom_left = vec3_add(vec3_add(center, bottom_offset), left_offset);

  int location = self->num_textures;
  int i;
  int found = 0;
  for (i = 0; i < self->num_textures; i++) {
    if (self->texture_ids[i] == texture_id) {
      location = i;
      found = 1;
      break;
    }
  }

  if (!found) {
    self->texture_ids[location] = texture_id;
    self->num_textures++;
  }

  i = self->num_texture_vertices[location];
  if (i < MAX_TEXTURE_VERTICES) {
    self->texture_vertices[location][i].position = top_left;
    self->texture_vertices[location][i].u = 0;
    self->texture_vertices[location][i++].v = 0;

    self->texture_vertices[location][i].position = bottom_left;
    self->texture_vertices[location][i].u = 0;
    self->texture_vertices[location][i++].v = 1;

    self->texture_vertices[location][i].position = bottom_right;
    self->texture_vertices[location][i].u = 1;
    self->texture_vertices[location][i++].v = 1;

    self->texture_vertices[location][i].position = top_left;
    self->texture_vertices[location][i].u = 0;
    self->texture_vertices[location][i++].v = 0;

    self->texture_vertices[location][i].position = top_right;
    self->texture_vertices[location][i].u = 1;
    self->texture_vertices[location][i++].v = 0;

    self->texture_vertices[location][i].position = bottom_right;
    self->texture_vertices[location][i].u = 1;
    self->texture_vertices[location][i++].v = 1;

    self->num_texture_vertices[location] = i;
  } else {
    printf("error! too many texture vertices\n");
  }
}
