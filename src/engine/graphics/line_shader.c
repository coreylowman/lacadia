#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "engine/util/camera.h"
#include "line_shader.h"

static void pre_render(Shader *self, Camera camera);
static void render(Shader *self, Camera camera);
static void post_render(Shader *self);

LineShader *line_shader_new() {
  LineShader *self = malloc(sizeof(*self));
  int result =
      shader_init(&self->base_shader, "./assets/shaders/line_vert.glsl",
                  "./assets/shaders/line_frag.glsl", pre_render, render, post_render);
  if (result) {
    printf("Error loading line shader\n");
    exit(result);
  }

  self->num_lines = 0;
  glGenVertexArrays(1, &self->vao);
  glBindVertexArray(self->vao);

  glGenBuffers(1, &self->vbo);

  glBindBuffer(GL_ARRAY_BUFFER, self->vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  self->projection_matrix_location =
      glGetUniformLocation(self->base_shader.program, "projection_matrix");
  self->view_matrix_location =
      glGetUniformLocation(self->base_shader.program, "view_matrix");

  return self;
}

void line_shader_free(LineShader *self) { free(self); }

static void pre_render(Shader *shader, Camera camera) {
  LineShader *self = (LineShader *)shader;
  glUseProgram(self->base_shader.program);
  glUniformMatrix4fv(self->projection_matrix_location, 1, GL_TRUE,
                     &camera.projection_matrix.data[0]);
  glUniformMatrix4fv(self->view_matrix_location, 1, GL_TRUE,
                     &camera.view_matrix.data[0]);
}

static void render(Shader *shader, Camera camera) {
  LineShader *self = (LineShader *)shader;
  glBindBuffer(GL_ARRAY_BUFFER, self->vbo);
  glBufferData(GL_ARRAY_BUFFER, 2 * 3 * self->num_lines * sizeof(float),
               &self->lines[0], GL_DYNAMIC_DRAW);

  glBindVertexArray(self->vao);
  glDrawArrays(GL_LINES, 0, 2 * self->num_lines);
  glBindVertexArray(0);
}

static void post_render(Shader *shader) {
  LineShader *self = (LineShader *)shader;
  self->num_lines = 0;
}

void line_shader_add_line(LineShader *self, Line line) {
  int i = self->num_lines;
  if (i < MAX_LINES) {
    self->lines[i] = line;
    self->num_lines++;
  } else {
    printf("error! too many lines\n");
  }
}
