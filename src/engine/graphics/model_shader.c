#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include "model_shader.h"
#include "engine/util/camera.h"

extern Vec3 light_position;

static void pre_render(Shader *self, Camera camera);
static void render(Shader *self, Camera camera);
static void post_render(Shader *self);

ModelShader *model_shader_new(int num_models, ObjectModel **models) {
  ModelShader *self = malloc(sizeof(*self));
  int result =
      shader_init(&self->base_shader, "./shaders/model_vert.glsl",
                  "./shaders/model_frag.glsl", pre_render, render, post_render);
  if (result) {
    printf("Error loading model shader\n");
    exit(result);
  }

  if (num_models >= MAX_MODELS) {
    printf("Too many models\n");
    exit(1);
  }

  self->num_models = num_models;
  int i;
  for (i = 0; i < num_models; i++) {
    self->models[i] = models[i];
    self->model_model_matrices[i] = array_list_new_m4();
  }
  for (i = num_models; i < MAX_MODELS; i++) {
    self->models[i] = NULL;
    self->model_model_matrices[i] = NULL;
  }

  glGenVertexArrays(1, &self->vao);
  glBindVertexArray(self->vao);

  glGenBuffers(2, &self->vbo[0]);

  glBindBuffer(GL_ARRAY_BUFFER, self->vbo[0]);

  // vertex data buffer object
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ObjectModelVertex),
                        (const GLvoid *)0);
  glEnableVertexAttribArray(0);

  // color data buffer object
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(ObjectModelVertex),
                        (const GLvoid *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  // normal data buffer object
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ObjectModelVertex),
                        (const GLvoid *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, self->vbo[1]);
  // instance model matrix buffer object
  for (i = 0; i < 4; i++) {
    glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float),
                          (const GLvoid *)(4 * i * sizeof(float)));
    glEnableVertexAttribArray(3 + i);
    glVertexAttribDivisor(3 + i, 1);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  self->projection_matrix_location =
      glGetUniformLocation(self->base_shader.program, "projection_matrix");
  self->view_matrix_location =
      glGetUniformLocation(self->base_shader.program, "view_matrix");
  self->light_position_location =
      glGetUniformLocation(self->base_shader.program, "light_position");

  return self;
}

void model_shader_free(ModelShader *self) {
  int i;
  for (i = 0; i < self->num_models; i++) {
    self->models[i] = NULL;
    array_list_free_m4(self->model_model_matrices[i]);
  }
  free(self);
}

static void pre_render(Shader *shader, Camera camera) {
  ModelShader *self = (ModelShader *)shader;
  glUseProgram(self->base_shader.program);
  glUniformMatrix4fv(self->projection_matrix_location, 1, GL_TRUE,
                     &camera.projection_matrix.data[0]);
  glUniformMatrix4fv(self->view_matrix_location, 1, GL_TRUE,
                     &camera.view_matrix.data[0]);
  glUniform3f(self->light_position_location, light_position.x, light_position.y,
              light_position.z);
}

static void render(Shader *shader, Camera camera) {
  ModelShader *self = (ModelShader *)shader;
  int i;
  for (i = 0; i < self->num_models; i++) {
    ObjectModel *model = self->models[i];
    ArrayList_m4 *model_matrices = self->model_model_matrices[i];

    if (model_matrices->length == 0)
      continue;

    // upload vertices of model
    glBindBuffer(GL_ARRAY_BUFFER, self->vbo[0]);
    glBufferData(GL_ARRAY_BUFFER,
                 model->num_vertices * sizeof(ObjectModelVertex),
                 &model->vertices[0], GL_DYNAMIC_DRAW);

    // upload instace model matrices
    glBindBuffer(GL_ARRAY_BUFFER, self->vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, model_matrices->length * sizeof(Mat4),
                 &model_matrices->data[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // draw stuff
    glBindVertexArray(self->vao);
    glDrawArraysInstanced(GL_TRIANGLES, 0, model->num_vertices,
                          model_matrices->length);
    glBindVertexArray(0);
  }
}

static void post_render(Shader *shader) {
  ModelShader *self = (ModelShader *)shader;
  int i;
  for (i = 0; i < self->num_models; i++) {
    self->model_model_matrices[i]->length = 0;
  }
}

void model_shader_add_model(ModelShader *self, int model_id,
                            Mat4 model_matrix) {
  array_list_push_m4(self->model_model_matrices[model_id], model_matrix);
}
