#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "terrain_shader.h"
#include "engine/util/camera.h"

extern Vec3 light_position;

static void pre_render(Shader *self, Camera camera);
static void render(Shader *self, Camera camera);
static void post_render(Shader *self);

TerrainShader *terrain_shader_new() {
    TerrainShader *self = malloc(sizeof(*self));
    self->num_terrains = 0;
  int result = shader_init(&self->base_shader, "./shaders/terrain_vert.glsl", "./shaders/terrain_frag.glsl",
    pre_render, render, post_render);

  if(result) {
        printf("Error loading terrain shader\n");
        exit(result);
    }

  glGenVertexArrays(1, &self->vao);
    glBindVertexArray(self->vao);

    glGenBuffers(1, &self->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, self->vbo);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex),
        (const GLvoid *)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex),
        (const GLvoid *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex),
        (const GLvoid *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

  self->projection_matrix_location = glGetUniformLocation(self->base_shader.program, "projection_matrix");
  self->view_matrix_location = glGetUniformLocation(self->base_shader.program, "view_matrix");
  self->light_position_location = glGetUniformLocation(self->base_shader.program, "light_position");
  return self;
}

void terrain_shader_free(TerrainShader *self) {
    free(self);
}

static void pre_render(Shader *shader, Camera camera) {
	TerrainShader *self = (TerrainShader *)shader;
    glUseProgram(self->base_shader.program);
    glUniformMatrix4fv(self->projection_matrix_location, 1,
        GL_TRUE, &camera.projection_matrix.data[0]);
    glUniformMatrix4fv(self->view_matrix_location, 1, GL_TRUE,
		&camera.view_matrix.data[0]);
    glUniform3f(self->light_position_location, light_position.x,
        light_position.y, light_position.z);
}

static void render(Shader *shader, Camera camera) {
	TerrainShader *self = (TerrainShader *)shader;
    int i;
    Terrain t;
    for (i = 0; i < self->num_terrains; i++) {
        t = self->terrains[i];
        glBindVertexArray(self->vao);
        glBindBuffer(GL_ARRAY_BUFFER, self->vbo);

        glBufferData(GL_ARRAY_BUFFER, t.num_vertices * sizeof(TerrainVertex),
            &t.vertices[0], GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, t.num_vertices);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}

static void post_render(Shader *shader) {
	TerrainShader *self = (TerrainShader *)shader;
    self->num_terrains = 0;
}

void terrain_shader_add_terrain(TerrainShader *self, Terrain terrain) {
      int i = self->num_terrains;
  if (i < MAX_TERRAINS) {
    self->terrains[i] = terrain;
    self->num_terrains++;
  } else {
    printf("error! too many lines\n");
  }
}
