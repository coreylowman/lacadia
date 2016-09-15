#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include "ft2build.h"
#include FT_FREETYPE_H
#include "lodepng.h"
#include "renderer.h"
#include "engine/util/string_helpers.h"

Vec3 light_position = {.data = {200, 100, 0}};

static int load_png_data(const char *filename, unsigned char **image,
                         unsigned *width, unsigned *height) {
  unsigned error;
  unsigned char *png = 0;
  size_t pngsize;

  error = lodepng_load_file(&png, &pngsize, filename);
  if (!error)
    error = lodepng_decode32(image, width, height, png, pngsize);
  if (error) {
    printf("error %u: %s\n", error, lodepng_error_text(error));
    free(png);
    return 0;
  }

  free(png);
  return 1;
}

static void render_text(Renderer *self);
static void init_text(Renderer *self);

Renderer *renderer_new() {
  Renderer *self = malloc(sizeof(*self));

  //
  // MODELS
  //
  shader_init(&self->model_shader, "./shaders/model_vert.glsl",
              "./shaders/model_frag.glsl");
  self->num_models = 11;
  self->model_names[0] = "./assets/default_box";
  self->model_names[1] = "./assets/mage";
  self->model_names[2] = "./assets/hunter";
  self->model_names[3] = "./assets/bug";
  self->model_names[4] = "./assets/fireball";
  self->model_names[5] = "./assets/icicle";
  self->model_names[6] = "./assets/burn_particle";
  self->model_names[7] = "./assets/wall";
  self->model_names[8] = "./assets/sphere";
  self->model_names[9] = "./assets/frost_particle";
  self->model_names[10] = "./assets/coin";

  printf("Loading models... ");
  int i;
  for (i = 0; i < self->num_models; i++) {
    self->models[i] = obj_model_from_file(self->model_names[i]);
    self->model_model_matrices[i] = array_list_new_m4();
  }
  printf("done.\n");

  //
  // Models
  //
  glGenVertexArrays(1, &self->model_vao);
  glBindVertexArray(self->model_vao);

  glGenBuffers(2, &self->model_vbo[0]);

  glBindBuffer(GL_ARRAY_BUFFER, self->model_vbo[0]);

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

  glBindBuffer(GL_ARRAY_BUFFER, self->model_vbo[1]);
  // instance model matrix buffer object
  for (i = 0; i < 4; i++) {
    glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, 16 * sizeof(float),
                          (const GLvoid *)(4 * i * sizeof(float)));
    glEnableVertexAttribArray(3 + i);
    glVertexAttribDivisor(3 + i, 1);
  }

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  //
  // UI
  //
  shader_init(&self->ui_shader, "./shaders/ui_vert.glsl",
              "./shaders/ui_frag.glsl");
  self->num_ui_rects = 0;
  // ui vbo/vao
  glGenVertexArrays(1, &self->ui_vao);
  glBindVertexArray(self->ui_vao);

  glGenBuffers(2, &self->ui_vbo[0]);

  // vertex data buffer object
  glBindBuffer(GL_ARRAY_BUFFER, self->ui_vbo[0]);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  // color data buffer object
  glBindBuffer(GL_ARRAY_BUFFER, self->ui_vbo[1]);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  //
  // LINES
  //
  shader_init(&self->line_shader, "./shaders/line_vert.glsl",
              "./shaders/line_frag.glsl");
  self->num_lines = 0;
  glGenVertexArrays(1, &self->line_vao);
  glBindVertexArray(self->line_vao);

  glGenBuffers(1, &self->line_vbo);

  glBindBuffer(GL_ARRAY_BUFFER, self->line_vbo);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  init_text(self);

  //
  // TERRAINS
  //
  shader_init(&self->terrain_shader, "./shaders/terrain_vert.glsl",
              "./shaders/terrain_frag.glsl");
  self->num_terrains = 0;
  glGenVertexArrays(1, &self->terrain_vao);
  glBindVertexArray(self->terrain_vao);

  glGenBuffers(1, &self->terrain_vbo);

  // vertex data buffer object
  glBindBuffer(GL_ARRAY_BUFFER, self->terrain_vbo);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex),
                        (const GLvoid *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex),
                        (const GLvoid *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(TerrainVertex),
                        (const GLvoid *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  unsigned png_width, png_height;

  // size should be 256x256
  if (!load_png_data("./assets/grass.2d.png", &self->textures[0], &png_width,
                     &png_height)) {
    printf("error: cannot load \"./assets/grass.2d.png\"\n");
  }

  if (!load_png_data("./assets/grass_dirt.2d.png", &self->textures[1],
                     &png_width, &png_height)) {
    printf("error: cannot load \"./assets/grass_dirt.2d.png\"\n");
  }

  if (!load_png_data("./assets/dirt.2d.png", &self->textures[2], &png_width,
                     &png_height)) {
    printf("error: cannot load \"./assets/dirt.2d.png\"\n");
  }

  glGenTextures(3, &self->texture_ids[0]);

  glBindTexture(GL_TEXTURE_2D, self->texture_ids[0]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, self->textures[0]);

  glBindTexture(GL_TEXTURE_2D, self->texture_ids[1]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, self->textures[1]);

  glBindTexture(GL_TEXTURE_2D, self->texture_ids[2]);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 256, 256, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, self->textures[2]);

  glBindTexture(GL_TEXTURE_2D, 0);

  return self;
}

// tutorial here : http://learnopengl.com/#!In-Practice/Text-Rendering
static void init_text(Renderer *self) {
  shader_init(&self->text_shader, "./shaders/text_vert.glsl",
              "./shaders/text_frag.glsl");

  glGenVertexArrays(1, &self->text_vao);
  glBindVertexArray(self->text_vao);

  glGenBuffers(1, &self->text_vbo);
  glBindBuffer(GL_ARRAY_BUFFER, self->text_vbo);

  // vertex/uv coordinates buffer object
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(TextCharacterVertex),
                        (const GLvoid *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TextCharacterVertex),
                        (const GLvoid *)(4 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  FT_Library ft_library;
  if (FT_Init_FreeType(&ft_library)) {
    printf("Error: Freetype could not init freetype library\n");
    exit(1);
  }

  FT_Face face;
  if (FT_New_Face(ft_library, "./assets/fonts/arial.ttf", 0, &face)) {
    printf("Error: Freetype failed to load font\n");
    exit(1);
  }

  FT_Set_Pixel_Sizes(face, 0, 48);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  printf("Loading characters...");
  GLubyte c;
  for (c = 0; c < 128; c++) {
    if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
      printf("Error: Freetype failed to load char %c\n", c);
      continue;
    }

    // Generate texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width,
                 face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                 face->glyph->bitmap.buffer);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Now store character for later use
    CharacterStats stat = {.textureID = texture,
                           .size[0] = face->glyph->bitmap.width,
                           .size[1] = face->glyph->bitmap.rows,
                           .bearing[0] = face->glyph->bitmap_left,
                           .bearing[1] = face->glyph->bitmap_top,
                           .advance = face->glyph->advance.x};
    // printf("Loaded '%c' with size (%d, %d) bearing (%d, %d), advance (%d)\n",
    // c,
    //        stat.size[0], stat.size[1], stat.bearing[0], stat.bearing[1],
    //        stat.advance);

    self->char_stats[c] = stat;
    self->num_character_vertices[c] = 0;
  }
  printf("done.\n");
  glBindTexture(GL_TEXTURE_2D, 0);

  FT_Done_Face(face);
  FT_Done_FreeType(ft_library);
}

void renderer_free(Renderer *self) {
  int i;
  for (i = 0; i < self->num_models; i++) {
    obj_model_free(self->models[i]);
    array_list_free_m4(self->model_model_matrices[i]);
  }
  free(self->textures[0]);
  free(self->textures[1]);
  free(self->textures[2]);
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

void renderer_render(Renderer *self, Mat4 projection_matrix, Mat4 view_matrix) {
  // render models
  glUseProgram(self->model_shader.program);
  glUniformMatrix4fv(self->model_shader.projection_matrix_location, 1, GL_TRUE,
                     &projection_matrix.data[0]);
  glUniformMatrix4fv(self->model_shader.view_matrix_location, 1, GL_TRUE,
                     &view_matrix.data[0]);
  glUniform3f(self->terrain_shader.light_position_location, light_position.x,
              light_position.y, light_position.z);

  int i;
  for (i = 0; i < self->num_models; i++) {
    ObjectModel *model = self->models[i];
    ArrayList_m4 *model_matrices = self->model_model_matrices[i];

    if (model_matrices->length == 0)
      continue;

    // upload vertices of model
    glBindBuffer(GL_ARRAY_BUFFER, self->model_vbo[0]);
    glBufferData(GL_ARRAY_BUFFER,
                 model->num_vertices * sizeof(ObjectModelVertex),
                 &model->vertices[0], GL_DYNAMIC_DRAW);

    // upload instace model matrices
    glBindBuffer(GL_ARRAY_BUFFER, self->model_vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, model_matrices->length * sizeof(Mat4),
                 &model_matrices->data[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // draw stuff
    glBindVertexArray(self->model_vao);
    glDrawArraysInstanced(GL_TRIANGLES, 0, model->num_vertices,
                          model_matrices->length);
    glBindVertexArray(0);

    self->model_model_matrices[i]->length = 0;
  }

  // render ui rects
  glUseProgram(self->ui_shader.program);

  // upload vertices of model
  glBindBuffer(GL_ARRAY_BUFFER, self->ui_vbo[0]);
  glBufferData(GL_ARRAY_BUFFER, 8 * self->num_ui_rects * sizeof(float),
               &self->rects[0], GL_DYNAMIC_DRAW);

  // upload colors of model
  glBindBuffer(GL_ARRAY_BUFFER, self->ui_vbo[1]);
  glBufferData(GL_ARRAY_BUFFER, 12 * self->num_ui_rects * sizeof(float),
               &self->rect_colors[0], GL_DYNAMIC_DRAW);

  glBindVertexArray(self->ui_vao);
  glDrawArrays(GL_QUADS, 0, 4 * self->num_ui_rects);
  glBindVertexArray(0);

  self->num_ui_rects = 0;

  // render lines
  glUseProgram(self->line_shader.program);
  glUniformMatrix4fv(self->line_shader.projection_matrix_location, 1, GL_TRUE,
                     &projection_matrix.data[0]);
  glUniformMatrix4fv(self->line_shader.view_matrix_location, 1, GL_TRUE,
                     &view_matrix.data[0]);

  glBindBuffer(GL_ARRAY_BUFFER, self->line_vbo);
  glBufferData(GL_ARRAY_BUFFER, 2 * 3 * self->num_lines * sizeof(float),
               &self->lines[0], GL_DYNAMIC_DRAW);

  glBindVertexArray(self->line_vao);
  glDrawArrays(GL_LINES, 0, 2 * self->num_lines);
  glBindVertexArray(0);

  self->num_lines = 0;

  // render terrain
  glUseProgram(self->terrain_shader.program);
  glUniformMatrix4fv(self->terrain_shader.projection_matrix_location, 1,
                     GL_TRUE, &projection_matrix.data[0]);
  glUniformMatrix4fv(self->terrain_shader.view_matrix_location, 1, GL_TRUE,
                     &view_matrix.data[0]);
  glUniform3f(self->terrain_shader.light_position_location, light_position.x,
              light_position.y, light_position.z);

  glUniform1i(self->terrain_shader.texture_location, 0);
  glActiveTexture(GL_TEXTURE0);

  Terrain t;
  for (i = 0; i < self->num_terrains; i++) {
    t = self->terrains[i];
    glBindVertexArray(self->terrain_vao);
    glBindBuffer(GL_ARRAY_BUFFER, self->terrain_vbo);

    glBufferData(GL_ARRAY_BUFFER, t.num_grass_vertices * sizeof(TerrainVertex),
                 &t.grass_vertices[0], GL_DYNAMIC_DRAW);
    glBindTexture(GL_TEXTURE_2D, self->texture_ids[0]);
    glDrawArrays(GL_QUADS, 0, t.num_grass_vertices);

    if (t.num_grass_dirt_vertices > 0) {
      glBufferData(GL_ARRAY_BUFFER,
                   t.num_grass_dirt_vertices * sizeof(TerrainVertex),
                   &t.grass_dirt_vertices[0], GL_DYNAMIC_DRAW);
      glBindTexture(GL_TEXTURE_2D, self->texture_ids[1]);
      glDrawArrays(GL_QUADS, 0, t.num_grass_dirt_vertices);
    }

    if (t.num_dirt_vertices > 0) {
      glBufferData(GL_ARRAY_BUFFER, t.num_dirt_vertices * sizeof(TerrainVertex),
                   &t.dirt_vertices[0], GL_DYNAMIC_DRAW);
      glBindTexture(GL_TEXTURE_2D, self->texture_ids[2]);
      glDrawArrays(GL_QUADS, 0, t.num_dirt_vertices);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
  }

  self->num_terrains = 0;

  // render text
  render_text(self);
}

void renderer_render_model(Renderer *self, int model_id, Mat4 model_matrix) {
  array_list_push_m4(self->model_model_matrices[model_id], model_matrix);

  // int i;
  // Vec3 a, b, n;
  // Line l;
  // ObjectModelVertex v;
  // ObjectModel *m = self->models[model_id];
  // Mat4 mm = model_matrix;
  // mat4_transpose(&mm);
  // for (i = 0; i < m->num_vertices; i += 250){
  //	v = m->vertices[i];
  //
  //	mat4_mul_vec3(&a, mm, vec3_from_3f(v.position[0], v.position[1],
  // v.position[2]));
  //	n = vec3_from_3f(v.normal[0], v.normal[1], v.normal[2]);
  //	b = vec3_add(a, n);
  //	l.start = a;
  //	l.end = b;
  //	renderer_render_line(self, l);
  //}
}

void renderer_render_rect(Renderer *self, Rect2 rect, Vec3 color) {
  int i = self->num_ui_rects;
  if (i < MAX_UI_RECTS) {
    self->rects[i] = ui_rect_from_rect2(rect);
    self->rect_colors[4 * i] = color;
    self->rect_colors[4 * i + 1] = color;
    self->rect_colors[4 * i + 2] = color;
    self->rect_colors[4 * i + 3] = color;
    self->num_ui_rects++;
  } else {
    printf("error! too many rects\n");
  }
}

void renderer_render_line(Renderer *self, Line line) {
  int i = self->num_lines;
  if (i < MAX_LINES) {
    self->lines[i] = line;
    self->num_lines++;
  } else {
    printf("error! too many lines\n");
  }
}

void renderer_render_terrain(Renderer *self, Terrain terrain) {
  int i = self->num_terrains;
  if (i < MAX_TERRAINS) {
    self->terrains[i] = terrain;
    self->num_terrains++;
  } else {
    printf("error! too many lines\n");
  }
}

void renderer_render_sphere(Renderer *self, Vec3 position) {
  int model_id = renderer_get_model_id(self, "./assets/sphere");
  Mat4 model_matrix;
  mat4_ident(&model_matrix);
  mat4_translate(&model_matrix, position);
  mat4_transpose(&model_matrix);
  renderer_render_model(self, model_id, model_matrix);
}

static float vertices[6][4] = {{0, 0, 0.0, 0.0},
                               {0, 0, 0.0, 1.0},
                               {0, 0, 1.0, 1.0},
                               {0, 0, 0.0, 0.0},
                               {0, 0, 1.0, 1.0},
                               {0, 0, 1.0, 0.0}};

static void fill_vertices(float x, float y, float w, float h) {
  vertices[0][0] = x;
  vertices[0][1] = y + h;

  vertices[1][0] = x;
  vertices[1][1] = y;

  vertices[2][0] = x + w;
  vertices[2][1] = y;

  vertices[3][0] = x;
  vertices[3][1] = y + h;

  vertices[4][0] = x + w;
  vertices[4][1] = y;

  vertices[5][0] = x + w;
  vertices[5][1] = y + h;
}

static void render_text(Renderer *self) {
  glUseProgram(self->text_shader.program);

  glUniform1i(glGetUniformLocation(self->text_shader.program, "text"), 0);
  glActiveTexture(GL_TEXTURE0);

  int i;
  for (i = 0; i < 128; i++) {
    if (self->num_character_vertices[i] == 0)
      continue;

    glBindVertexArray(self->text_vao);
    glBindTexture(GL_TEXTURE_2D, self->char_stats[i].textureID);

    glBindBuffer(GL_ARRAY_BUFFER, self->text_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 self->num_character_vertices[i] * sizeof(TextCharacterVertex),
                 &self->characters[i][0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArraysInstanced(GL_TRIANGLES, 0, self->num_character_vertices[i],
                          self->num_character_vertices[i] / 6);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);

    self->num_character_vertices[i] = 0;
  }
}

void renderer_render_text(Renderer *self, const char *buffer, int len,
                          Vec3 xyscale, Vec3 color) {
  float x = xyscale.x;
  float y = xyscale.y;
  float scale = xyscale.z;

  int i, j;
  char c;
  CharacterStats stat;
  for (i = 0; i < len; i++) {
    c = buffer[i];
    stat = self->char_stats[c];

    float xpos = x + stat.bearing[0] * scale;
    // note: multiple by 0.5 so the scaling in next section doesn't apply to the
    // offset
    float ypos = y - (stat.size[1] - stat.bearing[1]) * scale * 0.5;
    float w = stat.size[0] * scale;
    float h = stat.size[1] * scale;

    xpos = xpos * 2.0 / 1280.0 - 1;
    ypos = ypos * 2.0 / 960.0 - 1;
    w = w / 1280.0;
    h = h / 960.0;

    fill_vertices(xpos, ypos, w, h);

    for (j = 0; j < 6; j++) {
      if (self->num_character_vertices[c] < MAX_CHAR_VERTS) {
        self->characters[c][self->num_character_vertices[c]].x = vertices[j][0];
        self->characters[c][self->num_character_vertices[c]].y = vertices[j][1];
        self->characters[c][self->num_character_vertices[c]].u = vertices[j][2];
        self->characters[c][self->num_character_vertices[c]].v = vertices[j][3];
        self->characters[c][self->num_character_vertices[c]].color = color;
        self->num_character_vertices[c]++;
      } else {
        printf("Error: too many '%c'\n", c);
      }
    }

    // Now advance cursors for next glyph (note that advance is number of 1/64
    // pixels)
    x += (stat.advance >> 6) *
         scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
  }
}
