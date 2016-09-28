#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "engine/util/camera.h"
#include "water_shader.h"

#define REFLECTION_WIDTH 360
#define REFLECTION_HEIGHT 180

#define REFRACTION_WIDTH 1280
#define REFRACTION_HEIGHT 960

extern int width, height;

static void pre_render(Shader *self);
static void render(Shader *self);
static void post_render(Shader *self);

static unsigned int create_framebuffer() {
  unsigned int framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  glDrawBuffer(GL_COLOR_ATTACHMENT0);
  return framebuffer;
}

static unsigned int add_texture_attachment(int width, int height) {
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
               GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture, 0);
  return texture;
}

static unsigned int add_depth_texture_attachment(int width, int height) {
  unsigned int texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0,
               GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, texture, 0);
  return texture;
}

static unsigned int add_depth_buffer_attachment(int width, int height) {
  unsigned int depth_buffer;
  glGenRenderbuffers(1, &depth_buffer);
  glBindRenderbuffer(GL_RENDERBUFFER, depth_buffer);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                            GL_RENDERBUFFER, depth_buffer);
  return depth_buffer;
}

static void bind_framebuffer(int framebuffer, int width, int height) {
  glBindTexture(GL_TEXTURE_2D, 0);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
  glViewport(0, 0, width, height);
}

static void unbind_framebuffer() {
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, width, height);
}

WaterShader *water_shader_new(AssetManager *asset_manager) {
  WaterShader *self = malloc(sizeof(*self));
  int result = shader_init(
      &self->base_shader, "./assets/shaders/water_vert.glsl",
      "./assets/shaders/water_frag.glsl", pre_render, render, post_render);
  if (result) {
    printf("Error loading water shader\n");
    exit(result);
  }

  self->num_water_vertices = 0;
  glGenVertexArrays(1, &self->vao);
  glBindVertexArray(self->vao);

  glGenBuffers(1, &self->vbo);

  glBindBuffer(GL_ARRAY_BUFFER, self->vbo);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(WaterVertex), 0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(WaterVertex),
                        (const GLvoid *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  self->distortion_texture =
      asset_manager_get_texture_id(asset_manager, "waterDUDV");
  self->normal_texture =
      asset_manager_get_texture_id(asset_manager, "waterNormal");

  self->reflection_texture_location =
      glGetUniformLocation(self->base_shader.program, "reflection_texture");
  self->refraction_texture_location =
      glGetUniformLocation(self->base_shader.program, "refraction_texture");
  self->distortion_texture_location =
      glGetUniformLocation(self->base_shader.program, "distortion_texture");
  self->move_factor_location =
      glGetUniformLocation(self->base_shader.program, "move_factor");
  self->camera_position_location =
      glGetUniformLocation(self->base_shader.program, "camera_position");
  self->normal_texture_location =
      glGetUniformLocation(self->base_shader.program, "normal_texture");
  self->depth_texture_location =
      glGetUniformLocation(self->base_shader.program, "depth_texture");

  self->reflection_frame_buffer = create_framebuffer();
  self->reflection_texture =
      add_texture_attachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
  self->reflection_depth_buffer =
      add_depth_buffer_attachment(REFLECTION_WIDTH, REFLECTION_HEIGHT);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    printf("Error setting up framebuffer\n");
  }
  unbind_framebuffer();

  self->refraction_frame_buffer = create_framebuffer();
  self->refraction_texture =
      add_texture_attachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
  self->refraction_depth_texture =
      add_depth_texture_attachment(REFRACTION_WIDTH, REFRACTION_HEIGHT);
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
    printf("Error setting up framebuffer\n");
  }
  unbind_framebuffer();

  return self;
}

void water_shader_free(WaterShader *self) {
  glDeleteFramebuffers(1, &self->reflection_frame_buffer);
  glDeleteTextures(1, &self->reflection_texture);
  glDeleteRenderbuffers(1, &self->reflection_depth_buffer);
  glDeleteFramebuffers(1, &self->refraction_frame_buffer);
  glDeleteTextures(1, &self->refraction_texture);
  glDeleteTextures(1, &self->refraction_depth_texture);
  free(self);
}

static float wave_speed = 0.015f;
static float move_factor = 0.0f;
extern double DELTA_TIME;

static void pre_render(Shader *shader) {
  WaterShader *self = (WaterShader *)shader;
  move_factor += wave_speed * DELTA_TIME;
  if (move_factor >= 1.0) {
    move_factor = 0.0;
  }

  glUseProgram(shader->program);
  glUniformMatrix4fv(shader->projection_matrix_location, 1, GL_TRUE,
                     &shader->projection_matrix->data[0]);
  glUniformMatrix4fv(shader->view_matrix_location, 1, GL_TRUE,
                     &shader->view_matrix->data[0]);
  glUniform3f(shader->light_position_location, shader->light_position->x,
              shader->light_position->y, shader->light_position->z);
  glUniform3f(self->camera_position_location, self->camera_position->x,
              self->camera_position->y, self->camera_position->z);
  glUniform1f(self->move_factor_location, move_factor);

  glUniform1i(self->reflection_texture_location, 0);
  glUniform1i(self->refraction_texture_location, 1);
  glUniform1i(self->distortion_texture_location, 2);
  glUniform1i(self->normal_texture_location, 3);
  glUniform1i(self->depth_texture_location, 4);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, self->reflection_texture);
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, self->refraction_texture);
  glActiveTexture(GL_TEXTURE2);
  glBindTexture(GL_TEXTURE_2D, self->distortion_texture);
  glActiveTexture(GL_TEXTURE3);
  glBindTexture(GL_TEXTURE_2D, self->normal_texture);
  glActiveTexture(GL_TEXTURE4);
  glBindTexture(GL_TEXTURE_2D, self->refraction_depth_texture);
}

static void render(Shader *shader) {
  WaterShader *self = (WaterShader *)shader;
  glBindBuffer(GL_ARRAY_BUFFER, self->vbo);
  glBufferData(GL_ARRAY_BUFFER,
               2 * 3 * self->num_water_vertices * sizeof(float),
               &self->water_vertices[0], GL_DYNAMIC_DRAW);

  glBindVertexArray(self->vao);
  glDrawArrays(GL_TRIANGLES, 0, self->num_water_vertices);
  glBindVertexArray(0);
}

static void post_render(Shader *shader) {
  WaterShader *self = (WaterShader *)shader;
  self->num_water_vertices = 0;
}

void water_shader_add_chunk(WaterShader *self, Vec3 center, float width,
                            float length) {
  float w_2 = width * 0.5;
  float l_2 = length * 0.5;

  Vec3 top_left = center;
  top_left.x -= w_2;
  top_left.z -= l_2;

  Vec3 top_right = center;
  top_right.x += w_2;
  top_right.z -= l_2;

  Vec3 bottom_left = center;
  bottom_left.x -= w_2;
  bottom_left.z += l_2;

  Vec3 bottom_right = center;
  bottom_right.x += w_2;
  bottom_right.z += l_2;

  int i = self->num_water_vertices;
  if (i < MAX_WATER_CHUNKS) {
    self->water_vertices[i].position = top_left;
    self->water_vertices[i].u = 0;
    self->water_vertices[i++].v = 0;

    self->water_vertices[i].position = bottom_left;
    self->water_vertices[i].u = 0;
    self->water_vertices[i++].v = 1;

    self->water_vertices[i].position = bottom_right;
    self->water_vertices[i].u = 1;
    self->water_vertices[i++].v = 1;

    self->water_vertices[i].position = top_left;
    self->water_vertices[i].u = 0;
    self->water_vertices[i++].v = 0;

    self->water_vertices[i].position = top_right;
    self->water_vertices[i].u = 1;
    self->water_vertices[i++].v = 0;

    self->water_vertices[i].position = bottom_right;
    self->water_vertices[i].u = 1;
    self->water_vertices[i++].v = 1;

    self->num_water_vertices = i;
  } else {
    printf("error! too many waters\n");
  }
}

void water_shader_render_reflection(WaterShader *self, Renderer *renderer,
                                    WaterShaderRenderCallback renderCallback) {
  bind_framebuffer(self->reflection_frame_buffer, REFLECTION_WIDTH,
                   REFLECTION_HEIGHT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  renderCallback(renderer);
  unbind_framebuffer();
}

void water_shader_render_refraction(WaterShader *self, Renderer *renderer,
                                    WaterShaderRenderCallback renderCallback) {
  bind_framebuffer(self->refraction_frame_buffer, REFRACTION_WIDTH,
                   REFRACTION_HEIGHT);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  renderCallback(renderer);
  unbind_framebuffer();
}
