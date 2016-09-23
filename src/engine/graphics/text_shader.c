#include <stdlib.h>
#include <GL/glew.h>
#include "ft2build.h"
#include FT_FREETYPE_H
#include "text_shader.h"
#include "engine/util/camera.h"

static void pre_render(Shader *self, Camera camera, Vec3 clip_plane, float clip_dist);
static void render(Shader *self, Camera camera);
static void post_render(Shader *self);

TextShader *text_shader_new() {
  TextShader *self = malloc(sizeof(*self));

  int result =
      shader_init(&self->base_shader, "./assets/shaders/text_vert.glsl",
                  "./assets/shaders/text_frag.glsl", pre_render, render, post_render);
  if (result) {
    printf("Error loading text shader\n");
    exit(result);
  }

  glGenVertexArrays(1, &self->vao);
  glBindVertexArray(self->vao);

  glGenBuffers(1, &self->vbo);
  glBindBuffer(GL_ARRAY_BUFFER, self->vbo);

  // vertex/uv coordinates buffer object
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(TextCharacterVertex),
                        (const GLvoid *)0);
  glEnableVertexAttribArray(0);

  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(TextCharacterVertex),
                        (const GLvoid *)(4 * sizeof(float)));
  glEnableVertexAttribArray(1);

  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  self->texture_location = glGetUniformLocation(self->base_shader.program, "text");

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

    self->char_stats[c] = stat;
    self->num_character_vertices[c] = 0;
  }
  printf("done.\n");
  glBindTexture(GL_TEXTURE_2D, 0);

  FT_Done_Face(face);
  FT_Done_FreeType(ft_library);

  return self;
}

void text_shader_free(TextShader *self) { free(self); }

static void pre_render(Shader *shader, Camera camera, Vec3 clip_plane, float clip_dist) {
  TextShader *self = (TextShader *)shader;
  glUseProgram(self->base_shader.program);

  glUniform1i(self->texture_location, 0);
  glActiveTexture(GL_TEXTURE0);
}

static void render(Shader *shader, Camera camera) {
  TextShader *self = (TextShader *)shader;
  int i;
  for (i = 0; i < 128; i++) {
    if (self->num_character_vertices[i] == 0)
      continue;

    glBindVertexArray(self->vao);
    glBindTexture(GL_TEXTURE_2D, self->char_stats[i].textureID);

    glBindBuffer(GL_ARRAY_BUFFER, self->vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 self->num_character_vertices[i] * sizeof(TextCharacterVertex),
                 &self->characters[i][0], GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glDrawArraysInstanced(GL_TRIANGLES, 0, self->num_character_vertices[i],
                          self->num_character_vertices[i] / 6);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
  }
}

static void post_render(Shader *shader) {
  TextShader *self = (TextShader *)shader;
  int i;
  for (i = 0; i < 128; i++) {
    self->num_character_vertices[i] = 0;
  }
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

void text_shader_add_text(TextShader *self, const char *buffer, int len,
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
